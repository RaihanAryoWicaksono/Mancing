#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EconomyManager.generated.h"

USTRUCT(BlueprintType)
struct FFishMarketPrice
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FName FishID;
    UPROPERTY(BlueprintReadWrite) int64 CurrentPricePerKg = 0;
    UPROPERTY(BlueprintReadWrite) int64 BasePricePerKg = 0;
    UPROPERTY(BlueprintReadWrite) float DemandMultiplier = 1.f;
    UPROPERTY(BlueprintReadWrite) int32 TodaySoldCount = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPricesUpdated);

UCLASS()
class GAMEKU_API AEconomyManager : public AActor
{
    GENERATED_BODY()

public:
    AEconomyManager();

    virtual void BeginPlay() override;

    static AEconomyManager* GetInstance(UWorld* World);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    int64 GetFishPrice(FName FishID, float WeightKg) const;

    UFUNCTION(BlueprintCallable, Category = "Economy")
    int64 SellFish(FName FishID, float WeightKg);

    UFUNCTION(BlueprintCallable, Category = "Economy")
    void UpdateDailyPrices();

    UFUNCTION(BlueprintPure, Category = "Economy")
    float GetPriceMultiplier(FName FishID) const;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPricesUpdated OnPricesUpdated;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float OverfishingPenalty = 0.1f;    // Tiap 10 ekor dijual = -10% harga

    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float MaxPriceMultiplier = 2.5f;    // Max harga naik 150%

    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float MinPriceMultiplier = 0.3f;    // Min harga turun ke 30%

private:
    TMap<FName, FFishMarketPrice> MarketPrices;

    void InitializeDefaultPrices();
    float CalculateDemandFactor(FName FishID) const;
};
