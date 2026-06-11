#include "Core/GamekuGameMode.h"
#include "Character/FishingCharacter.h"
#include "Core/GamekuPlayerController.h"
#include "Environment/FishingZone.h"
#include "UObject/ConstructorHelpers.h"

AGamekuGameMode::AGamekuGameMode()
{
    PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
        TEXT("/Game/Blueprints/Character/BP_FishingCharacter"));
    if (PlayerPawnBPClass.Class != nullptr)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}

void AGamekuGameMode::BeginPlay()
{
    Super::BeginPlay();
    CurrentGameTime = 360.f; // Mulai jam 06:00
}

void AGamekuGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateGameTime(DeltaTime);
}

void AGamekuGameMode::UpdateGameTime(float DeltaTime)
{
    float TimeScale = 1440.f / DayDurationSeconds;
    CurrentGameTime += DeltaTime * TimeScale;

    if (CurrentGameTime >= 1440.f)
    {
        CurrentGameTime -= 1440.f;
        StartNewDay();
    }

    BroadcastTimeEvents();
}

void AGamekuGameMode::StartNewDay()
{
    CurrentDay++;
    UE_LOG(LogTemp, Log, TEXT("Hari baru dimulai: Hari ke-%d"), CurrentDay);
}

void AGamekuGameMode::BroadcastTimeEvents()
{
    float CurrentHour = CurrentGameTime / 60.f;

    if (FMath::FloorToInt(CurrentHour) != FMath::FloorToInt(LastBroadcastHour))
    {
        LastBroadcastHour = CurrentHour;

        for (auto& ZonePtr : RegisteredZones)
        {
            if (ZonePtr.IsValid())
            {
                ZonePtr->OnHourChanged(CurrentHour);
            }
        }
    }
}

bool AGamekuGameMode::IsNightTime() const
{
    float CurrentHour = CurrentGameTime / 60.f;
    return CurrentHour >= NightStartHour || CurrentHour < NightEndHour;
}

void AGamekuGameMode::RegisterFishingZone(UFishingZone* Zone)
{
    if (Zone)
    {
        RegisteredZones.AddUnique(Zone);
    }
}
