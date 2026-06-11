#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "FishDataAsset.generated.h"

UENUM(BlueprintType)
enum class EFishRarity : uint8
{
    Common      UMETA(DisplayName = "Umum"),
    Uncommon    UMETA(DisplayName = "Tidak Umum"),
    Rare        UMETA(DisplayName = "Langka"),
    Epic        UMETA(DisplayName = "Epik"),
    Legendary   UMETA(DisplayName = "Legendaris")
};

UENUM(BlueprintType)
enum class EFishHabitat : uint8
{
    Coastal     UMETA(DisplayName = "Pantai"),
    Reef        UMETA(DisplayName = "Karang"),
    OpenSea     UMETA(DisplayName = "Laut Terbuka"),
    DeepSea     UMETA(DisplayName = "Laut Dalam"),
    River       UMETA(DisplayName = "Sungai"),
    Mangrove    UMETA(DisplayName = "Mangrove"),
    Esturay     UMETA(DisplayName = "Muara")
};

UENUM(BlueprintType)
enum class EFishFightStyle : uint8
{
    Sprinter    UMETA(DisplayName = "Sprinter - Lari cepat"),
    Diver       UMETA(DisplayName = "Diver - Menyelam dalam"),
    Jumper      UMETA(DisplayName = "Jumper - Melompat"),
    Shaker      UMETA(DisplayName = "Shaker - Geleng kepala"),
    Steady      UMETA(DisplayName = "Steady - Bergerak stabil"),
    Aggressive  UMETA(DisplayName = "Aggressive - Serangan beruntun")
};

USTRUCT(BlueprintType)
struct FFishActivityHours
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, ClampMax=24))
    float PeakStartHour = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0, ClampMax=24))
    float PeakEndHour = 8.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0, ClampMax=1.0))
    float PeakActivityMultiplier = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0, ClampMax=1.0))
    float NightActivityMultiplier = 0.3f;
};

USTRUCT(BlueprintType)
struct FBaitPreference
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BaitID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin=0.0, ClampMax=3.0))
    float EffectivenessMultiplier = 1.f;
};

UCLASS(BlueprintType)
class GAMEKU_API UFishDataAsset : public UPrimaryDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FName FishID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FText FishNameID;       // "Ikan Kakap Merah"

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FText FishNameLatin;    // "Lutjanus campechanus"

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    FText FishDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    UTexture2D* FishIcon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Identity")
    USkeletalMesh* FishMesh;

    // --- Stats ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    EFishRarity Rarity = EFishRarity::Common;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    EFishHabitat Habitat = EFishHabitat::Coastal;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    EFishFightStyle FightStyle = EFishFightStyle::Steady;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta=(ClampMin=0.01))
    float MinWeightKg = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta=(ClampMin=0.01))
    float MaxWeightKg = 5.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta=(ClampMin=0, ClampMax=1))
    float SpawnChance = 0.5f;      // 0-1, base spawn probability

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float MinDepthMeters = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
    float MaxDepthMeters = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta=(ClampMin=1))
    float FishStrength = 1.f;      // Multiplier untuk fight difficulty

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats", meta=(ClampMin=1))
    float FishStamina = 100.f;     // HP fight

    // --- Economy ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economy", meta=(ClampMin=0))
    int64 BasePricePerKg = 25000;  // Rp per kg

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economy")
    bool bCanBeSold = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Economy")
    bool bIsProtected = false;     // Dilarang ditangkap (hiu paus, penyu, dll)

    // --- Behavior ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    FFishActivityHours ActivityHours;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    TArray<FBaitPreference> BaitPreferences;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    bool bIsSchooling = false;     // Bergerombol?

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Behavior")
    int32 SchoolSize = 1;

    // --- XP Reward ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward")
    int32 BaseXPReward = 10;

    virtual FPrimaryAssetId GetPrimaryAssetId() const override
    {
        return FPrimaryAssetId("FishData", FishID);
    }
};
