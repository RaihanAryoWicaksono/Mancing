#include "Core/GamekuPlayerController.h"
#include "Blueprint/UserWidget.h"

AGamekuPlayerController::AGamekuPlayerController()
{
}

void AGamekuPlayerController::BeginPlay()
{
    Super::BeginPlay();
    ShowFishingHUD();
}

void AGamekuPlayerController::ShowFishingHUD()
{
    if (FishingHUDClass && !ActiveHUDWidget)
    {
        ActiveHUDWidget = CreateWidget<UUserWidget>(this, FishingHUDClass);
        if (ActiveHUDWidget)
        {
            ActiveHUDWidget->AddToViewport();
        }
    }
    else if (ActiveHUDWidget)
    {
        ActiveHUDWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AGamekuPlayerController::HideFishingHUD()
{
    if (ActiveHUDWidget)
    {
        ActiveHUDWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AGamekuPlayerController::ShowInventory()
{
    HideFishingHUD();

    if (InventoryWidgetClass)
    {
        UUserWidget* InvWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
        if (InvWidget)
        {
            InvWidget->AddToViewport();
            SetInputMode(FInputModeUIOnly());
            bShowMouseCursor = true;
        }
    }
}

void AGamekuPlayerController::ShowMap()
{
    if (MapWidgetClass)
    {
        UUserWidget* MapWidget = CreateWidget<UUserWidget>(this, MapWidgetClass);
        if (MapWidget)
        {
            MapWidget->AddToViewport();
            SetInputMode(FInputModeUIOnly());
            bShowMouseCursor = true;
        }
    }
}

void AGamekuPlayerController::ShowPauseMenu()
{
    if (PauseMenuClass)
    {
        UUserWidget* PauseWidget = CreateWidget<UUserWidget>(this, PauseMenuClass);
        if (PauseWidget)
        {
            PauseWidget->AddToViewport();
            SetPause(true);
            SetInputMode(FInputModeUIOnly());
            bShowMouseCursor = true;
        }
    }
}

void AGamekuPlayerController::ShowFishCaughtNotification(FName FishID, float WeightKg, int64 Value)
{
    // Trigger event ke HUD Widget via Blueprint
    UE_LOG(LogTemp, Log, TEXT("Notifikasi: %s (%.2f kg) = Rp %lld"),
        *FishID.ToString(), WeightKg, Value);
}

void AGamekuPlayerController::ShowLineBrokeNotification()
{
    UE_LOG(LogTemp, Warning, TEXT("Notifikasi: Line putus!"));
}

void AGamekuPlayerController::ShowStormWarning()
{
    UE_LOG(LogTemp, Warning, TEXT("PERINGATAN: Badai mendekat! Kembali ke pelabuhan!"));
}
