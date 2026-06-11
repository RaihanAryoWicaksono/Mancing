#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FishingZone.generated.h"

class AFishingSpot;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMEKU_API UFishingZone : public UActorComponent
{
    GENERATED_BODY()

public:
    UFishingZone();

    UFUNCTION(BlueprintCallable, Category = "Zone")
    void OnHourChanged(float NewHour);

    UFUNCTION(BlueprintCallable, Category = "Zone")
    void RegisterSpot(AFishingSpot* Spot);

    UFUNCTION(BlueprintPure, Category = "Zone")
    FName GetZoneID() const { return ZoneID; }

    UFUNCTION(BlueprintPure, Category = "Zone")
    bool IsUnlocked() const { return bIsUnlocked; }

    UFUNCTION(BlueprintCallable, Category = "Zone")
    void UnlockZone();

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    FName ZoneID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    FText ZoneName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    int32 RequiredPlayerLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
    bool bIsUnlocked = false;

private:
    TArray<TWeakObjectPtr<AFishingSpot>> RegisteredSpots;
};
