#include "Fishing/FishingRodComponent.h"
#include "Fishing/FishBase.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/KismetMathLibrary.h"

UFishingRodComponent::UFishingRodComponent()
{
    PrimaryComponentTick.bCanEverTick = true;

    CurrentRodStats.RodID = FName("Rod_Bambu");
    CurrentRodStats.MaxCastDistance = 15.f;
    CurrentRodStats.LineStrength = 5.f;
    CurrentRodStats.ReelSpeed = 0.8f;
}

void UFishingRodComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UFishingRodComponent::TickComponent(float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (CurrentRodState)
    {
        case ERodState::LineInWater:
            UpdateWaiting(DeltaTime);
            break;
        case ERodState::FishHooked:
            BitingTimer += DeltaTime;
            if (BitingTimer >= BitingWindowSeconds)
            {
                // Ikan lepas jika tidak di-hook dalam waktu
                UE_LOG(LogTemp, Warning, TEXT("Ikan kabur! Terlambat hook."));
                CurrentRodState = ERodState::LineInWater;
                bIsBiting = false;
                BitingTimer = 0.f;
                HookedFish.Reset();
            }
            break;
        case ERodState::Reeling:
            UpdateFighting(DeltaTime);
            break;
        default:
            break;
    }
}

void UFishingRodComponent::Cast(float Power)
{
    if (CurrentRodState != ERodState::Idle)
        return;

    Power = FMath::Clamp(Power, 0.f, 1.f);
    CurrentCastDistance = Power * CurrentRodStats.MaxCastDistance;
    CurrentTension = 0.f;

    float WaitTime = FMath::RandRange(WaitForBiteTimeMin, WaitForBiteTimeMax);
    // Umpan bagus dan skill mempengaruhi waktu tunggu
    WaitTimer = WaitTime;

    CurrentRodState = ERodState::LineInWater;
    UE_LOG(LogTemp, Log, TEXT("Cast! Jarak: %.1fm. Menunggu gigitan..."), CurrentCastDistance);
}

void UFishingRodComponent::StartReeling()
{
    bIsReeling = true;
    if (CurrentRodState == ERodState::FishHooked)
    {
        CurrentRodState = ERodState::Reeling;
    }
}

void UFishingRodComponent::StopReeling()
{
    bIsReeling = false;
}

void UFishingRodComponent::PullUp()
{
    bIsPullingUp = true;
}

void UFishingRodComponent::LetLine()
{
    // Beri slack — mengurangi tension tapi ikan bisa lari lebih jauh
    CurrentTension = FMath::Max(0.f, CurrentTension - 15.f);
}

bool UFishingRodComponent::TryHook()
{
    if (CurrentRodState != ERodState::FishHooked || !bIsBiting)
        return false;

    // Hook berhasil!
    bIsBiting = false;
    CurrentRodState = ERodState::Reeling;
    BitingTimer = 0.f;
    UE_LOG(LogTemp, Log, TEXT("HOOK! Ikan terkail!"));
    return true;
}

void UFishingRodComponent::UpdateWaiting(float DeltaTime)
{
    WaitTimer -= DeltaTime;

    if (WaitTimer <= 0.f)
    {
        float SpawnChance = CalculateFishSpawnChance();
        if (FMath::FRand() <= SpawnChance)
        {
            TriggerBite();
        }
        else
        {
            // Coba lagi setelah beberapa saat
            WaitTimer = FMath::RandRange(WaitForBiteTimeMin, WaitForBiteTimeMax);
        }
    }
}

void UFishingRodComponent::TriggerBite()
{
    bIsBiting = true;
    BitingTimer = 0.f;
    CurrentRodState = ERodState::FishHooked;

    // Spawn ikan (di BP_FishingSpot yang akan handle spawn actual fish AI)
    // Di sini kita hanya trigger event, FishingSpot yang spawn ikan
    UE_LOG(LogTemp, Log, TEXT("GIGITAN! Tekan hook sekarang!"));
    OnFishBite.Broadcast(nullptr); // Fish actor dibroadcast dari FishingSpot
}

void UFishingRodComponent::UpdateFighting(float DeltaTime)
{
    if (!HookedFish.IsValid())
    {
        CatchFish();
        return;
    }

    AFishBase* Fish = HookedFish.Get();

    // Ikan berjuang melawan reel
    float FishResistance = Fish->GetCurrentStamina() / 100.f * Fish->GetFishStrength();

    if (bIsReeling)
    {
        float ReelForce = CurrentRodStats.ReelSpeed;
        float NetForce = ReelForce - FishResistance;

        if (NetForce > 0.f)
        {
            CurrentTension += NetForce * DeltaTime * 20.f;
        }
        else
        {
            // Ikan lebih kuat dari reel — tension meningkat berbahaya
            CurrentTension += FMath::Abs(NetForce) * DeltaTime * 30.f;
        }
    }
    else
    {
        // Tidak reel = ikan menarik line
        CurrentTension += FishResistance * DeltaTime * 10.f;
    }

    // Kurangi stamina ikan
    Fish->DrainStamina(DeltaTime * (bIsPullingUp ? 1.5f : 1.f));

    CurrentTension = FMath::Clamp(CurrentTension, 0.f, MaxTension + 20.f);
    OnTensionChanged.Broadcast(GetNormalizedTension());

    if (CurrentTension >= MaxTension)
    {
        BreakLine();
        return;
    }

    // Tension alami berkurang saat tidak reel
    if (!bIsReeling)
    {
        CurrentTension = FMath::Max(0.f, CurrentTension - DeltaTime * 5.f);
    }

    if (Fish->GetCurrentStamina() <= 0.f)
    {
        CatchFish();
    }

    bIsPullingUp = false;
}

void UFishingRodComponent::CatchFish()
{
    if (HookedFish.IsValid())
    {
        OnFishCaught.Broadcast(HookedFish.Get());
        UE_LOG(LogTemp, Log, TEXT("IKAN TERTANGKAP!"));
    }

    ResetRod();
}

void UFishingRodComponent::BreakLine()
{
    UE_LOG(LogTemp, Warning, TEXT("LINE PUTUS! Tension terlalu tinggi."));
    OnLineBroke.Broadcast();
    HookedFish.Reset();
    ResetRod();
}

void UFishingRodComponent::ResetRod()
{
    CurrentRodState = ERodState::Idle;
    CurrentTension = 0.f;
    bIsReeling = false;
    bIsBiting = false;
    bIsPullingUp = false;
    WaitTimer = 0.f;
    BitingTimer = 0.f;
    HookedFish.Reset();
}

void UFishingRodComponent::EquipRod(FRodStats NewRodStats)
{
    if (CurrentRodState != ERodState::Idle)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tidak bisa ganti joran saat memancing!"));
        return;
    }
    CurrentRodStats = NewRodStats;
}

void UFishingRodComponent::SetBait(FName BaitID)
{
    CurrentBaitID = BaitID;
}

float UFishingRodComponent::GetNormalizedTension() const
{
    return FMath::Clamp(CurrentTension / MaxTension, 0.f, 1.f);
}

float UFishingRodComponent::CalculateFishSpawnChance() const
{
    // Base 40%, modifikasi dari skill, cuaca, waktu, umpan
    return 0.4f;
}
