#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GamekuGameMode.generated.h"

class UFishingZone;
class AGamekuPlayerController;

UCLASS()
class GAMEKU_API AGamekuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AGamekuGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Game")
    void StartNewDay();

    UFUNCTION(BlueprintCallable, Category = "Game")
    int32 GetCurrentDay() const { return CurrentDay; }

    UFUNCTION(BlueprintCallable, Category = "Game")
    float GetGameTime() const { return CurrentGameTime; }

    UFUNCTION(BlueprintPure, Category = "Game")
    bool IsNightTime() const;

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void RegisterFishingZone(UFishingZone* Zone);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
    float DayDurationSeconds = 1440.f; // 24 menit real = 1 game day

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
    float NightStartHour = 20.f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Time")
    float NightEndHour = 5.f;

private:
    float CurrentGameTime = 0.f; // 0-1440 (menit dalam sehari)
    int32 CurrentDay = 1;

    TArray<TWeakObjectPtr<UFishingZone>> RegisteredZones;

    void UpdateGameTime(float DeltaTime);
    void BroadcastTimeEvents();

    float LastBroadcastHour = -1.f;
};
