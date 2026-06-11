#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Core/GamekuGameInstance.h"
#include "GamekuSaveGame.generated.h"

UCLASS()
class GAMEKU_API UGamekuSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(SaveGame, BlueprintReadWrite)
    FPlayerStats SavedPlayerStats;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    int32 SavedDay = 1;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    float SavedGameTime = 360.f;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    FVector SavedPlayerLocation = FVector::ZeroVector;

    UPROPERTY(SaveGame, BlueprintReadWrite)
    FName SavedActiveBoat = FName("Boat_PerahuDayung");
};
