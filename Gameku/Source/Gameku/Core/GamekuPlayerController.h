#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GamekuPlayerController.generated.h"

class UFishingHUD;

UCLASS()
class GAMEKU_API AGamekuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AGamekuPlayerController();

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowFishingHUD();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void HideFishingHUD();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowInventory();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowMap();

    UFUNCTION(BlueprintCallable, Category = "UI")
    void ShowPauseMenu();

    UFUNCTION(BlueprintCallable, Category = "Notification")
    void ShowFishCaughtNotification(FName FishID, float WeightKg, int64 Value);

    UFUNCTION(BlueprintCallable, Category = "Notification")
    void ShowLineBrokeNotification();

    UFUNCTION(BlueprintCallable, Category = "Notification")
    void ShowStormWarning();

protected:
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> FishingHUDClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> InventoryWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MapWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> PauseMenuClass;

private:
    UPROPERTY()
    UUserWidget* ActiveHUDWidget = nullptr;

    UPROPERTY()
    UUserWidget* ActiveInventoryWidget = nullptr;
};
