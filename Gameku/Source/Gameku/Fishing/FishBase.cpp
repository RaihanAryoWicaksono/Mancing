#include "Fishing/FishBase.h"

AFishBase::AFishBase()
{
    PrimaryActorTick.bCanEverTick = true;

    FishMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FishMesh"));
    RootComponent = FishMeshComponent;
}

void AFishBase::BeginPlay()
{
    Super::BeginPlay();

    if (FishData)
    {
        InitializeFromData(FishData);
    }
}

void AFishBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsFighting)
    {
        UpdateFighting(DeltaTime);
    }
    else
    {
        UpdateSwimming(DeltaTime);
    }
}

void AFishBase::InitializeFromData(UFishDataAsset* Data)
{
    if (!Data)
        return;

    FishData = Data;

    // Generate berat acak dalam range
    GeneratedWeightKg = FMath::RandRange(Data->MinWeightKg, Data->MaxWeightKg);

    // Stamina proporsional terhadap berat
    float WeightFactor = GeneratedWeightKg / Data->MaxWeightKg;
    MaxStamina = Data->FishStamina * WeightFactor;
    CurrentStamina = MaxStamina;

    if (Data->FishMesh)
    {
        FishMeshComponent->SetSkeletalMesh(Data->FishMesh);
        // Scale berdasarkan berat — bigger fish = bigger mesh
        float ScaleFactor = FMath::Sqrt(WeightFactor) * 0.5f + 0.5f;
        SetActorScale3D(FVector(ScaleFactor));
    }
}

void AFishBase::DrainStamina(float Amount)
{
    CurrentStamina = FMath::Max(0.f, CurrentStamina - Amount);
}

float AFishBase::GetFishStrength() const
{
    if (!FishData)
        return 1.f;

    float WeightFactor = GeneratedWeightKg / FishData->MaxWeightKg;
    return FishData->FishStrength * WeightFactor;
}

void AFishBase::StartFighting()
{
    bIsFighting = true;
}

void AFishBase::StopFighting()
{
    bIsFighting = false;
}

EFishFightStyle AFishBase::GetFightStyle() const
{
    return FishData ? FishData->FightStyle : EFishFightStyle::Steady;
}

int64 AFishBase::GetSellPrice() const
{
    if (!FishData || !FishData->bCanBeSold)
        return 0;

    return FMath::RoundToInt64(FishData->BasePricePerKg * GeneratedWeightKg);
}

int32 AFishBase::GetXPReward() const
{
    if (!FishData)
        return 0;

    // XP lebih besar untuk ikan yang lebih besar dan lebih langka
    float WeightBonus = (GeneratedWeightKg / FishData->MaxWeightKg);
    float RarityMultiplier = 1.f + (int32)FishData->Rarity * 0.5f;
    return FMath::RoundToInt(FishData->BaseXPReward * WeightBonus * RarityMultiplier);
}

void AFishBase::UpdateSwimming(float DeltaTime)
{
    SwimTimer -= DeltaTime;
    if (SwimTimer <= 0.f)
    {
        // Pilih target renang baru secara acak
        SwimTarget = GetActorLocation() + FVector(
            FMath::RandRange(-500.f, 500.f),
            FMath::RandRange(-500.f, 500.f),
            FMath::RandRange(-50.f, 0.f)
        );
        SwimTimer = FMath::RandRange(2.f, 6.f);
    }

    FVector Direction = (SwimTarget - GetActorLocation()).GetSafeNormal();
    float SwimSpeed = FishData ? 100.f * FishData->FishStrength : 100.f;
    SetActorLocation(GetActorLocation() + Direction * SwimSpeed * DeltaTime);
}

void AFishBase::UpdateFighting(float DeltaTime)
{
    if (!FishData)
        return;

    // Fight behavior berbeda per FightStyle
    switch (FishData->FightStyle)
    {
        case EFishFightStyle::Sprinter:
            // Lari cepat ke samping
            break;
        case EFishFightStyle::Diver:
            // Menyelam ke bawah
            break;
        case EFishFightStyle::Jumper:
            // Melompat ke atas (efek visual)
            break;
        default:
            break;
    }
}
