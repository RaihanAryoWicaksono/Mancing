#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fishing/FishDataAsset.h"
#include "FishingSpot.generated.h"

class AFishBase;
class USphereComponent;
class UDecalComponent;

USTRUCT(BlueprintType)
struct FFishSpawnEntry
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UFishDataAsset* FishData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0, ClampMax=1.0))
    float SpawnWeight = 1.f;   // Relative probability
};

UCLASS()
class GAMEKU_API AFishingSpot : public AActor
{
    GENERATED_BODY()

public:
    AFishingSpot();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "FishingSpot")
    AFishBase* SpawnRandomFish();

    UFUNCTION(BlueprintCallable, Category = "FishingSpot")
    UFishDataAsset* SelectRandomFishData() const;

    UFUNCTION(BlueprintCallable, Category = "FishingSpot")
    bool IsPlayerInRange(AActor* Player) const;

    UFUNCTION(BlueprintCallable, Category = "FishingSpot")
    void OnHourChanged(float NewHour);

    UFUNCTION(BlueprintPure, Category = "FishingSpot")
    bool HasFishAvailable() const { return CurrentFishCount > 0; }

    UFUNCTION(BlueprintPure, Category = "FishingSpot")
    int32 GetCurrentFishCount() const { return CurrentFishCount; }

    UFUNCTION(BlueprintCallable, Category = "FishingSpot")
    void NotifyFishCaught();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USphereComponent* DetectionSphere;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDecalComponent* SpotDecal;   // Visual indicator di air

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    TArray<FFishSpawnEntry> PossibleFish;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    int32 MaxFishCount = 10;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    float FishRegenerationTime = 300.f;  // 5 menit untuk respawn

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    float SpotRadius = 1000.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    FName RequiredZone;   // Zone yang harus di-unlock untuk akses spot ini

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    int32 RequiredPlayerLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    float PeakHourStart = 5.f;  // Jam peak fishing

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    float PeakHourEnd = 8.f;

private:
    int32 CurrentFishCount = 0;
    float RegenerationTimer = 0.f;
    bool bIsInPeakHour = false;

    void UpdateRegeneration(float DeltaTime);
    float GetCurrentSpawnRate() const;
};
