#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fishing/FishDataAsset.h"
#include "FishBase.generated.h"

UCLASS(Abstract)
class GAMEKU_API AFishBase : public AActor
{
    GENERATED_BODY()

public:
    AFishBase();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Fish")
    void InitializeFromData(UFishDataAsset* Data);

    UFUNCTION(BlueprintCallable, Category = "Fish")
    void DrainStamina(float Amount);

    UFUNCTION(BlueprintPure, Category = "Fish")
    float GetCurrentStamina() const { return CurrentStamina; }

    UFUNCTION(BlueprintPure, Category = "Fish")
    float GetFishStrength() const;

    UFUNCTION(BlueprintPure, Category = "Fish")
    float GetGeneratedWeight() const { return GeneratedWeightKg; }

    UFUNCTION(BlueprintPure, Category = "Fish")
    UFishDataAsset* GetFishData() const { return FishData; }

    UFUNCTION(BlueprintCallable, Category = "Fish")
    void StartFighting();

    UFUNCTION(BlueprintCallable, Category = "Fish")
    void StopFighting();

    UFUNCTION(BlueprintPure, Category = "Fish")
    EFishFightStyle GetFightStyle() const;

    UFUNCTION(BlueprintPure, Category = "Fish")
    int64 GetSellPrice() const;

    UFUNCTION(BlueprintPure, Category = "Fish")
    int32 GetXPReward() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* FishMeshComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
    UFishDataAsset* FishData;

private:
    float CurrentStamina = 100.f;
    float MaxStamina = 100.f;
    float GeneratedWeightKg = 1.f;
    bool bIsFighting = false;

    void UpdateSwimming(float DeltaTime);
    void UpdateFighting(float DeltaTime);
    FVector SwimTarget;
    float SwimTimer = 0.f;
};
