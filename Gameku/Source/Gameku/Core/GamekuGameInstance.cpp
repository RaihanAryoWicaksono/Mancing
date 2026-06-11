#include "Core/GamekuGameInstance.h"
#include "Core/GamekuSaveGame.h"
#include "Kismet/GameplayStatics.h"

const TArray<int32> UGamekuGameInstance::LevelXPThresholds = {
    0, 100, 250, 500, 900, 1400, 2100, 3000, 4200, 5700,      // 1-10
    7500, 9800, 12600, 16000, 20100, 25000, 31000, 38200, 46700, 56700, // 11-20
    68400, 82000, 97700, 115700, 136200, 159400, 185500, 214700, 247300, 283600 // 21-30
};

void UGamekuGameInstance::Init()
{
    Super::Init();
    LoadGame();
}

void UGamekuGameInstance::SaveGame()
{
    if (!CurrentSaveGame)
    {
        CurrentSaveGame = Cast<UGamekuSaveGame>(
            UGameplayStatics::CreateSaveGameObject(UGamekuSaveGame::StaticClass()));
    }

    CurrentSaveGame->SavedPlayerStats = PlayerStats;
    UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
    UE_LOG(LogTemp, Log, TEXT("Game tersimpan."));
}

void UGamekuGameInstance::LoadGame()
{
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
    {
        CurrentSaveGame = Cast<UGamekuSaveGame>(
            UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

        if (CurrentSaveGame)
        {
            PlayerStats = CurrentSaveGame->SavedPlayerStats;
            UE_LOG(LogTemp, Log, TEXT("Game dimuat. Level: %d"), PlayerStats.Level);
        }
    }
    else
    {
        // Data awal untuk pemain baru
        PlayerStats.UnlockedZones.Add(FName("Zone_TelukKampung"));
    }
}

bool UGamekuGameInstance::HasSaveGame() const
{
    return UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0);
}

void UGamekuGameInstance::AddExperience(int32 Amount)
{
    PlayerStats.Experience += Amount;
    CheckLevelUp();
}

void UGamekuGameInstance::CheckLevelUp()
{
    int32 MaxLevel = LevelXPThresholds.Num();
    while (PlayerStats.Level < MaxLevel)
    {
        int32 NextLevelXP = LevelXPThresholds[PlayerStats.Level]; // index = next level
        if (PlayerStats.Experience >= NextLevelXP)
        {
            PlayerStats.Level++;
            UE_LOG(LogTemp, Log, TEXT("LEVEL UP! Sekarang Level %d"), PlayerStats.Level);
        }
        else
        {
            break;
        }
    }
}

void UGamekuGameInstance::AddMoney(int64 Amount)
{
    PlayerStats.Money += Amount;
}

bool UGamekuGameInstance::SpendMoney(int64 Amount)
{
    if (PlayerStats.Money < Amount)
    {
        return false;
    }
    PlayerStats.Money -= Amount;
    return true;
}

void UGamekuGameInstance::RegisterFishCaught(FName FishID, float Weight)
{
    PlayerStats.TotalFishCaught++;

    if (!PlayerStats.DiscoveredFish.Contains(FishID))
    {
        PlayerStats.DiscoveredFish.Add(FishID);
        UE_LOG(LogTemp, Log, TEXT("Ikan baru ditemukan: %s"), *FishID.ToString());
    }

    if (Weight > PlayerStats.HeaviestFish)
    {
        PlayerStats.HeaviestFish = Weight;
    }
}

bool UGamekuGameInstance::HasDiscoveredFish(FName FishID) const
{
    return PlayerStats.DiscoveredFish.Contains(FishID);
}

int32 UGamekuGameInstance::GetXPForNextLevel() const
{
    if (PlayerStats.Level < LevelXPThresholds.Num())
    {
        return LevelXPThresholds[PlayerStats.Level] - PlayerStats.Experience;
    }
    return 0;
}
