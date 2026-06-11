#include "Environment/WeatherSystem.h"
#include "Kismet/GameplayStatics.h"

AWeatherSystem::AWeatherSystem()
{
    PrimaryActorTick.bCanEverTick = true;

    WeatherProbabilities = {
        { EWeatherType::Sunny,     0.35f },
        { EWeatherType::Cloudy,    0.25f },
        { EWeatherType::Overcast,  0.15f },
        { EWeatherType::LightRain, 0.12f },
        { EWeatherType::HeavyRain, 0.07f },
        { EWeatherType::Storm,     0.04f },
        { EWeatherType::Foggy,     0.02f }
    };
}

void AWeatherSystem::BeginPlay()
{
    Super::BeginPlay();
    CurrentWeather = CreateWeatherData(EWeatherType::Sunny);
    TransitionProgress = 1.f;
}

void AWeatherSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeSinceLastChange += DeltaTime;

    if (TransitionProgress < 1.f)
    {
        UpdateTransition(DeltaTime);
    }

    if (TimeSinceLastChange >= WeatherChangeCooldown)
    {
        if (FMath::FRand() < 0.3f) // 30% chance per check
        {
            GenerateRandomWeather();
        }
    }
}

AWeatherSystem* AWeatherSystem::GetInstance(UWorld* World)
{
    if (!World)
        return nullptr;

    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(World, AWeatherSystem::StaticClass(), Found);
    return Found.IsEmpty() ? nullptr : Cast<AWeatherSystem>(Found[0]);
}

void AWeatherSystem::SetWeather(EWeatherType NewType, float TransitionTime)
{
    TargetWeather = CreateWeatherData(NewType);
    TransitionDuration = FMath::Max(1.f, TransitionTime);
    TransitionProgress = 0.f;
    TimeSinceLastChange = 0.f;
}

void AWeatherSystem::GenerateRandomWeather()
{
    float TotalProb = 0.f;
    for (const auto& Pair : WeatherProbabilities)
    {
        TotalProb += Pair.Value;
    }

    float Roll = FMath::FRandRange(0.f, TotalProb);
    float Accumulated = 0.f;

    for (const auto& Pair : WeatherProbabilities)
    {
        Accumulated += Pair.Value;
        if (Roll <= Accumulated)
        {
            if (Pair.Key != CurrentWeather.Type)
            {
                SetWeather(Pair.Key);
            }
            return;
        }
    }
}

void AWeatherSystem::UpdateTransition(float DeltaTime)
{
    TransitionProgress += DeltaTime / TransitionDuration;
    TransitionProgress = FMath::Min(TransitionProgress, 1.f);

    // Interpolate weather values
    CurrentWeather.WindSpeedKmh = FMath::Lerp(
        CurrentWeather.WindSpeedKmh, TargetWeather.WindSpeedKmh, DeltaTime / TransitionDuration);
    CurrentWeather.WaveHeightM = FMath::Lerp(
        CurrentWeather.WaveHeightM, TargetWeather.WaveHeightM, DeltaTime / TransitionDuration);

    if (TransitionProgress >= 1.f)
    {
        CurrentWeather = TargetWeather;
        OnWeatherChanged.Broadcast(CurrentWeather);
        ApplyWeatherEffects();
        UE_LOG(LogTemp, Log, TEXT("Cuaca berubah: %d"), (int32)CurrentWeather.Type);
    }
}

void AWeatherSystem::ApplyWeatherEffects()
{
    // Di sini kita akan trigger particle systems, wind, sky changes, dll
    // Ini akan di-extend di Blueprint
}

FWeatherData AWeatherSystem::CreateWeatherData(EWeatherType Type) const
{
    FWeatherData Data;
    Data.Type = Type;

    switch (Type)
    {
        case EWeatherType::Sunny:
            Data.WindSpeedKmh = FMath::RandRange(2.f, 10.f);
            Data.WaveHeightM = 0.2f;
            Data.FishBiteMultiplier = 1.0f;
            Data.BoatSpeedMultiplier = 1.0f;
            Data.bIsDangerous = false;
            break;

        case EWeatherType::Cloudy:
            Data.WindSpeedKmh = FMath::RandRange(5.f, 20.f);
            Data.WaveHeightM = 0.5f;
            Data.FishBiteMultiplier = 1.1f;
            Data.BoatSpeedMultiplier = 0.95f;
            Data.bIsDangerous = false;
            break;

        case EWeatherType::LightRain:
            Data.WindSpeedKmh = FMath::RandRange(10.f, 30.f);
            Data.WaveHeightM = 1.0f;
            Data.FishBiteMultiplier = 1.25f;   // Hujan = ikan aktif
            Data.BoatSpeedMultiplier = 0.85f;
            Data.bIsDangerous = false;
            break;

        case EWeatherType::HeavyRain:
            Data.WindSpeedKmh = FMath::RandRange(30.f, 50.f);
            Data.WaveHeightM = 2.0f;
            Data.FishBiteMultiplier = 0.8f;
            Data.BoatSpeedMultiplier = 0.6f;
            Data.bIsDangerous = false;
            break;

        case EWeatherType::Storm:
            Data.WindSpeedKmh = FMath::RandRange(60.f, 100.f);
            Data.WaveHeightM = FMath::RandRange(4.f, 8.f);
            Data.FishBiteMultiplier = 0.2f;
            Data.BoatSpeedMultiplier = 0.3f;
            Data.bIsDangerous = true;
            break;

        case EWeatherType::Foggy:
            Data.WindSpeedKmh = FMath::RandRange(0.f, 5.f);
            Data.WaveHeightM = 0.1f;
            Data.FishBiteMultiplier = 0.7f;
            Data.BoatSpeedMultiplier = 0.7f;
            Data.bIsDangerous = false;
            break;

        default:
            break;
    }

    return Data;
}
