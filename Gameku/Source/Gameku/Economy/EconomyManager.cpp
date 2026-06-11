#include "Economy/EconomyManager.h"
#include "Kismet/GameplayStatics.h"

AEconomyManager::AEconomyManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AEconomyManager::BeginPlay()
{
    Super::BeginPlay();
    InitializeDefaultPrices();
}

AEconomyManager* AEconomyManager::GetInstance(UWorld* World)
{
    if (!World)
        return nullptr;

    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(World, AEconomyManager::StaticClass(), FoundActors);
    return FoundActors.IsEmpty() ? nullptr : Cast<AEconomyManager>(FoundActors[0]);
}

void AEconomyManager::InitializeDefaultPrices()
{
    // Harga default berdasarkan GDD (Rp per kg)
    TMap<FName, int64> DefaultPrices = {
        { FName("F001"), 25000 },   // Bandeng
        { FName("F002"), 30000 },   // Kembung
        { FName("F010"), 85000 },   // Kakap Merah
        { FName("F011"), 120000 },  // Kerapu Macan
        { FName("F012"), 200000 },  // Kerapu Bebek
        { FName("F013"), 500000 },  // Napoleon
        { FName("F020"), 35000 },   // Tongkol
        { FName("F022"), 65000 },   // Tenggiri
        { FName("F023"), 90000 },   // Tuna Sirip Kuning
        { FName("F024"), 300000 },  // Marlin Biru
    };

    for (const auto& Pair : DefaultPrices)
    {
        FFishMarketPrice Price;
        Price.FishID = Pair.Key;
        Price.BasePricePerKg = Pair.Value;
        Price.CurrentPricePerKg = Pair.Value;
        Price.DemandMultiplier = 1.f;
        MarketPrices.Add(Pair.Key, Price);
    }
}

int64 AEconomyManager::GetFishPrice(FName FishID, float WeightKg) const
{
    if (!MarketPrices.Contains(FishID))
        return 0;

    const FFishMarketPrice& Price = MarketPrices[FishID];
    return FMath::RoundToInt64(Price.CurrentPricePerKg * WeightKg);
}

int64 AEconomyManager::SellFish(FName FishID, float WeightKg)
{
    int64 Earned = GetFishPrice(FishID, WeightKg);

    if (MarketPrices.Contains(FishID))
    {
        FFishMarketPrice& Price = MarketPrices[FishID];
        Price.TodaySoldCount++;

        // Supply naik = harga turun sedikit
        float SupplyPenalty = Price.TodaySoldCount * OverfishingPenalty * 0.01f;
        float NewMultiplier = FMath::Clamp(Price.DemandMultiplier - SupplyPenalty,
            MinPriceMultiplier, MaxPriceMultiplier);

        Price.DemandMultiplier = NewMultiplier;
        Price.CurrentPricePerKg = FMath::RoundToInt64(Price.BasePricePerKg * NewMultiplier);
    }

    return Earned;
}

void AEconomyManager::UpdateDailyPrices()
{
    for (auto& Pair : MarketPrices)
    {
        FFishMarketPrice& Price = Pair.Value;
        Price.TodaySoldCount = 0;

        // Harga recover sedikit tiap hari
        float Recovery = 0.05f;
        Price.DemandMultiplier = FMath::Clamp(Price.DemandMultiplier + Recovery,
            MinPriceMultiplier, MaxPriceMultiplier);

        // Random faktor (festival, musim, dll)
        float RandomFactor = FMath::RandRange(-0.05f, 0.1f);
        Price.DemandMultiplier = FMath::Clamp(Price.DemandMultiplier + RandomFactor,
            MinPriceMultiplier, MaxPriceMultiplier);

        Price.CurrentPricePerKg = FMath::RoundToInt64(Price.BasePricePerKg * Price.DemandMultiplier);
    }

    OnPricesUpdated.Broadcast();
}

float AEconomyManager::GetPriceMultiplier(FName FishID) const
{
    if (!MarketPrices.Contains(FishID))
        return 1.f;

    return MarketPrices[FishID].DemandMultiplier;
}

float AEconomyManager::CalculateDemandFactor(FName FishID) const
{
    return 1.f; // Extend nanti dengan musim, festival, dll
}
