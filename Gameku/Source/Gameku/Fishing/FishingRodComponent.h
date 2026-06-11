#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishingRodComponent.generated.h"

class AFishBase;
class UInventoryComponent;

UENUM(BlueprintType)
enum class ERodState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Casting     UMETA(DisplayName = "Mengayun"),
    LineInWater UMETA(DisplayName = "Line di Air"),
    FishHooked  UMETA(DisplayName = "Ikan Terkail"),
    Reeling     UMETA(DisplayName = "Menarik"),
    FishCaught  UMETA(DisplayName = "Ikan Tertangkap"),
    LineBroke   UMETA(DisplayName = "Line Putus")
};

USTRUCT(BlueprintType)
struct FRodStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite) FName RodID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float MaxCastDistance = 30.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float LineStrength = 10.f;   // kg
    UPROPERTY(EditAnywhere, BlueprintReadWrite) float ReelSpeed = 1.f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 RequiredLevel = 1;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFishBite, AFishBase*, Fish);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFishCaught, AFishBase*, Fish);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLineBroke);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTensionChanged, float, NormalizedTension);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEKU_API UFishingRodComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFishingRodComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType,
        FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void Cast(float Power);       // Power: 0.0 - 1.0

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void StartReeling();

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void StopReeling();

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void PullUp();                // Angkat joran ke atas

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void LetLine();               // Kasih slack ke line

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    bool TryHook();               // Player tekan hook saat ikan gigit

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void ResetRod();

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void EquipRod(FRodStats NewRodStats);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void SetBait(FName BaitID);

    UFUNCTION(BlueprintPure, Category = "Fishing")
    ERodState GetRodState() const { return CurrentRodState; }

    UFUNCTION(BlueprintPure, Category = "Fishing")
    float GetTension() const { return CurrentTension; }

    UFUNCTION(BlueprintPure, Category = "Fishing")
    float GetNormalizedTension() const;

    UFUNCTION(BlueprintPure, Category = "Fishing")
    float GetCastDistance() const { return CurrentCastDistance; }

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnFishBite OnFishBite;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnFishCaught OnFishCaught;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLineBroke OnLineBroke;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTensionChanged OnTensionChanged;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
    FRodStats CurrentRodStats;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float BitingWindowSeconds = 1.5f;   // Waktu untuk meng-hook ikan

    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float MaxTension = 100.f;

    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float TensionDangerThreshold = 80.f;  // Tension > ini = berbahaya

    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float WaitForBiteTimeMin = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "Config")
    float WaitForBiteTimeMax = 30.f;

private:
    ERodState CurrentRodState = ERodState::Idle;

    FName CurrentBaitID;
    float CurrentCastDistance = 0.f;
    float CurrentTension = 0.f;
    bool bIsReeling = false;
    bool bIsPullingUp = false;

    float WaitTimer = 0.f;
    float BitingTimer = 0.f;
    bool bIsBiting = false;

    TWeakObjectPtr<AFishBase> HookedFish;

    void UpdateWaiting(float DeltaTime);
    void UpdateFighting(float DeltaTime);
    void TriggerBite();
    void CatchFish();
    void BreakLine();
    float CalculateFishSpawnChance() const;
};
