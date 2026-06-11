#include "Progression/SkillTreeComponent.h"

TArray<FSkillData> USkillTreeComponent::AllSkillDefinitions;

USkillTreeComponent::USkillTreeComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    InitSkillDefinitions();
}

void USkillTreeComponent::InitSkillDefinitions()
{
    if (!AllSkillDefinitions.IsEmpty())
        return;

    auto MakeSkill = [](FName ID, FText Name, ESkillBranch Branch, int32 MaxLv, int32 ReqLv,
        FName Prereq, TArray<int32> Costs) -> FSkillData
    {
        FSkillData S;
        S.SkillID = ID;
        S.SkillName = Name;
        S.Branch = Branch;
        S.MaxLevel = MaxLv;
        S.RequiredPlayerLevel = ReqLv;
        S.PrerequisiteSkill = Prereq;
        S.SkillPointCost = Costs;
        return S;
    };

    // === FISHING BRANCH ===
    AllSkillDefinitions.Add(MakeSkill("SK_PowerCast", FText::FromString("Power Cast"),
        ESkillBranch::Fishing, 5, 1, NAME_None, {1,1,2,2,3}));

    AllSkillDefinitions.Add(MakeSkill("SK_FishingSense", FText::FromString("Indera Memancing"),
        ESkillBranch::Fishing, 5, 3, FName("SK_PowerCast"), {1,2,2,3,3}));

    AllSkillDefinitions.Add(MakeSkill("SK_IronGrip", FText::FromString("Cengkeraman Besi"),
        ESkillBranch::Fishing, 5, 5, FName("SK_PowerCast"), {1,2,2,3,3}));

    AllSkillDefinitions.Add(MakeSkill("SK_QuickHook", FText::FromString("Hook Cepat"),
        ESkillBranch::Fishing, 3, 3, NAME_None, {1,2,3}));

    AllSkillDefinitions.Add(MakeSkill("SK_Patience", FText::FromString("Kesabaran"),
        ESkillBranch::Fishing, 5, 1, NAME_None, {1,1,1,2,2}));

    AllSkillDefinitions.Add(MakeSkill("SK_NightFishing", FText::FromString("Ahli Malam"),
        ESkillBranch::Fishing, 3, 8, FName("SK_Patience"), {2,3,4}));

    AllSkillDefinitions.Add(MakeSkill("SK_DeepSea", FText::FromString("Penguasa Laut Dalam"),
        ESkillBranch::Fishing, 1, 15, FName("SK_IronGrip"), {5}));

    // === BOAT BRANCH ===
    AllSkillDefinitions.Add(MakeSkill("SK_Navigation", FText::FromString("Navigasi"),
        ESkillBranch::Boat, 5, 1, NAME_None, {1,1,2,2,3}));

    AllSkillDefinitions.Add(MakeSkill("SK_StormRider", FText::FromString("Penunggang Badai"),
        ESkillBranch::Boat, 3, 10, FName("SK_Navigation"), {3,4,5}));

    AllSkillDefinitions.Add(MakeSkill("SK_FuelEfficiency", FText::FromString("Hemat BBM"),
        ESkillBranch::Boat, 3, 5, FName("SK_Navigation"), {2,2,3}));

    // === BUSINESS BRANCH ===
    AllSkillDefinitions.Add(MakeSkill("SK_Bargaining", FText::FromString("Tawar-Menawar"),
        ESkillBranch::Business, 5, 1, NAME_None, {1,1,2,2,3}));

    AllSkillDefinitions.Add(MakeSkill("SK_MarketSense", FText::FromString("Insting Pasar"),
        ESkillBranch::Business, 3, 5, FName("SK_Bargaining"), {2,3,4}));

    AllSkillDefinitions.Add(MakeSkill("SK_ExportLicense", FText::FromString("Lisensi Ekspor"),
        ESkillBranch::Business, 1, 12, FName("SK_MarketSense"), {5}));
}

bool USkillTreeComponent::UnlockSkill(FName SkillID)
{
    if (HasSkill(SkillID))
        return false;

    if (!CanUnlockSkill(SkillID))
        return false;

    const FSkillData* Data = FindSkillData(SkillID);
    if (!Data || Data->SkillPointCost.IsEmpty())
        return false;

    int32 Cost = Data->SkillPointCost[0];
    if (AvailableSkillPoints < Cost)
        return false;

    AvailableSkillPoints -= Cost;

    FUnlockedSkill NewSkill;
    NewSkill.SkillID = SkillID;
    NewSkill.CurrentLevel = 1;
    UnlockedSkills.Add(NewSkill);

    OnSkillUnlocked.Broadcast(SkillID, 1);
    return true;
}

bool USkillTreeComponent::UpgradeSkill(FName SkillID)
{
    for (FUnlockedSkill& Skill : UnlockedSkills)
    {
        if (Skill.SkillID == SkillID)
        {
            const FSkillData* Data = FindSkillData(SkillID);
            if (!Data)
                return false;

            if (Skill.CurrentLevel >= Data->MaxLevel)
                return false;

            int32 Cost = Data->SkillPointCost[Skill.CurrentLevel]; // Next level cost
            if (AvailableSkillPoints < Cost)
                return false;

            AvailableSkillPoints -= Cost;
            Skill.CurrentLevel++;
            OnSkillUnlocked.Broadcast(SkillID, Skill.CurrentLevel);
            return true;
        }
    }
    return false;
}

bool USkillTreeComponent::HasSkill(FName SkillID) const
{
    return GetSkillLevel(SkillID) > 0;
}

int32 USkillTreeComponent::GetSkillLevel(FName SkillID) const
{
    for (const FUnlockedSkill& Skill : UnlockedSkills)
    {
        if (Skill.SkillID == SkillID)
            return Skill.CurrentLevel;
    }
    return 0;
}

bool USkillTreeComponent::CanUnlockSkill(FName SkillID) const
{
    const FSkillData* Data = FindSkillData(SkillID);
    if (!Data)
        return false;

    if (!Data->PrerequisiteSkill.IsNone() && !HasSkill(Data->PrerequisiteSkill))
        return false;

    return true;
}

void USkillTreeComponent::AddSkillPoints(int32 Amount)
{
    AvailableSkillPoints += Amount;
}

float USkillTreeComponent::GetCastDistanceBonus() const
{
    return GetSkillLevel(FName("SK_PowerCast")) * 0.1f; // +10% per level
}

float USkillTreeComponent::GetBiteWaitReduction() const
{
    return GetSkillLevel(FName("SK_Patience")) * 0.08f; // -8% per level
}

float USkillTreeComponent::GetLineStrengthBonus() const
{
    return GetSkillLevel(FName("SK_IronGrip")) * 0.15f; // +15% per level
}

float USkillTreeComponent::GetSellPriceBonus() const
{
    return GetSkillLevel(FName("SK_Bargaining")) * 0.05f; // +5% per level
}

float USkillTreeComponent::GetBoatSpeedBonus() const
{
    return GetSkillLevel(FName("SK_Navigation")) * 0.05f; // +5% per level
}

const FSkillData* USkillTreeComponent::FindSkillData(FName SkillID) const
{
    for (const FSkillData& Skill : AllSkillDefinitions)
    {
        if (Skill.SkillID == SkillID)
            return &Skill;
    }
    return nullptr;
}
