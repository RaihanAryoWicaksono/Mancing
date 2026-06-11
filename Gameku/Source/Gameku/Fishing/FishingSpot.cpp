#include "Fishing/FishingSpot.h"
#include "Fishing/FishBase.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFishingSpot::AFishingSpot()
{
    PrimaryActorTick.bCanEverTick = true;

    DetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
    DetectionSphere->SetSphereRadius(SpotRadius);
    RootComponent = DetectionSphere;

    SpotDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SpotDecal"));
    SpotDecal->SetupAttachment(RootComponent);
}

void AFishingSpot::BeginPlay()
{
    Super::BeginPlay();
    CurrentFishCount = MaxFishCount;
}

void AFishingSpot::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateRegeneration(DeltaTime);
}

AFishBase* AFishingSpot::SpawnRandomFish()
{
    if (CurrentFishCount <= 0 || PossibleFish.IsEmpty())
        return nullptr;

    UFishDataAsset* SelectedData = SelectRandomFishData();
    if (!SelectedData)
        return nullptr;

    FVector SpawnLoc = GetActorLocation() + FVector(
        FMath::RandRange(-SpotRadius * 0.5f, SpotRadius * 0.5f),
        FMath::RandRange(-SpotRadius * 0.5f, SpotRadius * 0.5f),
        FMath::RandRange(-200.f, -50.f)
    );

    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

    // Spawn base fish class — BP override per species
    AFishBase* SpawnedFish = GetWorld()->SpawnActor<AFishBase>(
        AFishBase::StaticClass(), SpawnLoc, FRotator::ZeroRotator, SpawnParams);

    if (SpawnedFish)
    {
        SpawnedFish->InitializeFromData(SelectedData);
    }

    return SpawnedFish;
}

UFishDataAsset* AFishingSpot::SelectRandomFishData() const
{
    if (PossibleFish.IsEmpty())
        return nullptr;

    float TotalWeight = 0.f;
    for (const FFishSpawnEntry& Entry : PossibleFish)
    {
        if (Entry.FishData)
        {
            TotalWeight += Entry.SpawnWeight;
        }
    }

    float RandomValue = FMath::FRandRange(0.f, TotalWeight);
    float Accumulated = 0.f;

    for (const FFishSpawnEntry& Entry : PossibleFish)
    {
        if (!Entry.FishData)
            continue;

        Accumulated += Entry.SpawnWeight;
        if (RandomValue <= Accumulated)
        {
            // Cek activity hours
            return Entry.FishData;
        }
    }

    return PossibleFish.Last().FishData;
}

bool AFishingSpot::IsPlayerInRange(AActor* Player) const
{
    if (!Player)
        return false;

    float Distance = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
    return Distance <= SpotRadius;
}

void AFishingSpot::OnHourChanged(float NewHour)
{
    bIsInPeakHour = (NewHour >= PeakHourStart && NewHour < PeakHourEnd);
}

void AFishingSpot::NotifyFishCaught()
{
    CurrentFishCount = FMath::Max(0, CurrentFishCount - 1);
    UE_LOG(LogTemp, Log, TEXT("Ikan ditangkap dari spot ini. Sisa: %d/%d"),
        CurrentFishCount, MaxFishCount);
}

void AFishingSpot::UpdateRegeneration(float DeltaTime)
{
    if (CurrentFishCount >= MaxFishCount)
        return;

    RegenerationTimer += DeltaTime;
    if (RegenerationTimer >= FishRegenerationTime)
    {
        RegenerationTimer = 0.f;
        CurrentFishCount = FMath::Min(MaxFishCount, CurrentFishCount + 1);
    }
}

float AFishingSpot::GetCurrentSpawnRate() const
{
    return bIsInPeakHour ? 2.f : 1.f;
}
