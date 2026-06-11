#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeatherSystem.generated.h"

class UExponentialHeightFogComponent;
class UDirectionalLightComponent;

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
    Sunny       UMETA(DisplayName = "Cerah"),
    Cloudy      UMETA(DisplayName = "Berawan"),
    Overcast    UMETA(DisplayName = "Mendung"),
    LightRain   UMETA(DisplayName = "Hujan Ringan"),
    HeavyRain   UMETA(DisplayName = "Hujan Deras"),
    Storm       UMETA(DisplayName = "Badai"),
    Foggy       UMETA(DisplayName = "Berkabut")
};

USTRUCT(BlueprintType)
struct FWeatherData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) EWeatherType Type = EWeatherType::Sunny;
    UPROPERTY(BlueprintReadWrite) float WindSpeedKmh = 5.f;
    UPROPERTY(BlueprintReadWrite) float WaveHeightM = 0.2f;
    UPROPERTY(BlueprintReadWrite) float FishBiteMultiplier = 1.f;
    UPROPERTY(BlueprintReadWrite) float BoatSpeedMultiplier = 1.f;
    UPROPERTY(BlueprintReadWrite) bool bIsDangerous = false;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, FWeatherData, NewWeather);

UCLASS()
class GAMEKU_API AWeatherSystem : public AActor
{
    GENERATED_BODY()

public:
    AWeatherSystem();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    static AWeatherSystem* GetInstance(UWorld* World);

    UFUNCTION(BlueprintCallable, Category = "Weather")
    void SetWeather(EWeatherType NewType, float TransitionTime = 30.f);

    UFUNCTION(BlueprintCallable, Category = "Weather")
    void GenerateRandomWeather();

    UFUNCTION(BlueprintPure, Category = "Weather")
    FWeatherData GetCurrentWeather() const { return CurrentWeather; }

    UFUNCTION(BlueprintPure, Category = "Weather")
    bool IsStormActive() const { return CurrentWeather.Type == EWeatherType::Storm; }

    UFUNCTION(BlueprintPure, Category = "Weather")
    float GetFishBiteMultiplier() const { return CurrentWeather.FishBiteMultiplier; }

    UFUNCTION(BlueprintPure, Category = "Weather")
    float GetWindSpeed() const { return CurrentWeather.WindSpeedKmh; }

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnWeatherChanged OnWeatherChanged;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float WeatherChangeCooldown = 600.f; // Min 10 menit antar perubahan cuaca

    // Probabilitas tiap cuaca (total harus 1.0)
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    TMap<EWeatherType, float> WeatherProbabilities;

private:
    FWeatherData CurrentWeather;
    FWeatherData TargetWeather;
    float TransitionProgress = 1.f;
    float TransitionDuration = 30.f;
    float TimeSinceLastChange = 0.f;

    void UpdateTransition(float DeltaTime);
    void ApplyWeatherEffects();
    FWeatherData CreateWeatherData(EWeatherType Type) const;
};
