#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Fishing/FishDataAsset.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FCaughtFishInstance
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FName FishID;
    UPROPERTY(BlueprintReadWrite) float WeightKg = 0.f;
    UPROPERTY(BlueprintReadWrite) int64 EstimatedValue = 0;
    UPROPERTY(BlueprintReadWrite) FDateTime CaughtTime;
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FName ItemID;
    UPROPERTY(BlueprintReadWrite) FText ItemName;
    UPROPERTY(BlueprintReadWrite) int32 Quantity = 0;
    UPROPERTY(BlueprintReadWrite) int64 UnitPrice = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStorageFull, float, CurrentWeight);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEKU_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddCaughtFish(const FCaughtFishInstance& FishInstance);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void RemoveAllFish();

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool AddItem(FName ItemID, int32 Quantity = 1);

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    bool RemoveItem(FName ItemID, int32 Quantity = 1);

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool HasItem(FName ItemID, int32 Quantity = 1) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int32 GetItemCount(FName ItemID) const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    float GetCurrentFishWeightKg() const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    float GetMaxStorageKg() const { return MaxStorageKg; }

    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool IsStorageFull() const;

    UFUNCTION(BlueprintPure, Category = "Inventory")
    int64 GetTotalFishValue() const;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    int64 SellAllFish();   // Returns total money earned

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TArray<FCaughtFishInstance> CaughtFish;

    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    TArray<FInventoryItem> Items;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnInventoryChanged OnInventoryChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStorageFull OnStorageFull;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
    float MaxStorageKg = 30.f;   // Default storage kapal awal

private:
    float CachedFishWeight = 0.f;
};
