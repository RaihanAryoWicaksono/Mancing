#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillBranch : uint8
{
    Fishing     UMETA(DisplayName = "Keahlian Mancing"),
    Boat        UMETA(DisplayName = "Keahlian Kapal"),
    Business    UMETA(DisplayName = "Keahlian Bisnis")
};

USTRUCT(BlueprintType)
struct FSkillData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName SkillID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText SkillName;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) ESkillBranch Branch = ESkillBranch::Fishing;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 MaxLevel = 5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredPlayerLevel = 1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName PrerequisiteSkill;     // Skill yang harus di-unlock dulu
    UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<int32> SkillPointCost; // Cost per level
};

USTRUCT(BlueprintType)
struct FUnlockedSkill
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite) FName SkillID;
    UPROPERTY(BlueprintReadWrite) int32 CurrentLevel = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillUnlocked, FName, SkillID, int32, NewLevel);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEKU_API USkillTreeComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USkillTreeComponent();

    UFUNCTION(BlueprintCallable, Category = "Skills")
    bool UnlockSkill(FName SkillID);

    UFUNCTION(BlueprintCallable, Category = "Skills")
    bool UpgradeSkill(FName SkillID);

    UFUNCTION(BlueprintPure, Category = "Skills")
    bool HasSkill(FName SkillID) const;

    UFUNCTION(BlueprintPure, Category = "Skills")
    int32 GetSkillLevel(FName SkillID) const;

    UFUNCTION(BlueprintPure, Category = "Skills")
    bool CanUnlockSkill(FName SkillID) const;

    UFUNCTION(BlueprintPure, Category = "Skills")
    int32 GetAvailableSkillPoints() const { return AvailableSkillPoints; }

    UFUNCTION(BlueprintCallable, Category = "Skills")
    void AddSkillPoints(int32 Amount);

    // Getter untuk efek skill - dipanggil dari sistem lain
    UFUNCTION(BlueprintPure, Category = "Skills|Fishing")
    float GetCastDistanceBonus() const;

    UFUNCTION(BlueprintPure, Category = "Skills|Fishing")
    float GetBiteWaitReduction() const;

    UFUNCTION(BlueprintPure, Category = "Skills|Fishing")
    float GetLineStrengthBonus() const;

    UFUNCTION(BlueprintPure, Category = "Skills|Business")
    float GetSellPriceBonus() const;

    UFUNCTION(BlueprintPure, Category = "Skills|Boat")
    float GetBoatSpeedBonus() const;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSkillUnlocked OnSkillUnlocked;

private:
    TArray<FUnlockedSkill> UnlockedSkills;
    int32 AvailableSkillPoints = 0;

    // Definisi semua skill yang tersedia
    static TArray<FSkillData> AllSkillDefinitions;
    static void InitSkillDefinitions();

    const FSkillData* FindSkillData(FName SkillID) const;
};
