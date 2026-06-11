#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GamekuGameInstance.generated.h"

class UGamekuSaveGame;

USTRUCT(BlueprintType)
struct FPlayerStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) int32 Level = 1;
    UPROPERTY(BlueprintReadWrite) int32 Experience = 0;
    UPROPERTY(BlueprintReadWrite) int64 Money = 50000; // Rp 50.000 modal awal
    UPROPERTY(BlueprintReadWrite) int32 TotalFishCaught = 0;
    UPROPERTY(BlueprintReadWrite) float HeaviestFish = 0.f;
    UPROPERTY(BlueprintReadWrite) TArray<FName> UnlockedZones;
    UPROPERTY(BlueprintReadWrite) TArray<FName> DiscoveredFish;
    UPROPERTY(BlueprintReadWrite) TArray<FName> UnlockedSkills;
};

UCLASS()
class GAMEKU_API UGamekuGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;

    UFUNCTION(BlueprintCallable, Category = "Save")
    void SaveGame();

    UFUNCTION(BlueprintCallable, Category = "Save")
    void LoadGame();

    UFUNCTION(BlueprintCallable, Category = "Save")
    bool HasSaveGame() const;

    UFUNCTION(BlueprintCallable, Category = "Player")
    void AddExperience(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Player")
    void AddMoney(int64 Amount);

    UFUNCTION(BlueprintCallable, Category = "Player")
    bool SpendMoney(int64 Amount);

    UFUNCTION(BlueprintCallable, Category = "Player")
    void RegisterFishCaught(FName FishID, float Weight);

    UFUNCTION(BlueprintCallable, Category = "Player")
    bool HasDiscoveredFish(FName FishID) const;

    UFUNCTION(BlueprintPure, Category = "Player")
    int32 GetXPForNextLevel() const;

    UPROPERTY(BlueprintReadOnly, Category = "Player")
    FPlayerStats PlayerStats;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    FString SaveSlotName = TEXT("NelayarSave_01");

private:
    void CheckLevelUp();

    UPROPERTY()
    UGamekuSaveGame* CurrentSaveGame = nullptr;

    static const TArray<int32> LevelXPThresholds;
};
