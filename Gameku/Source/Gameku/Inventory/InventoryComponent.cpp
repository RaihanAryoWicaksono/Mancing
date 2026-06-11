#include "Inventory/InventoryComponent.h"
#include "Core/GamekuGameInstance.h"
#include "Kismet/GameplayStatics.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

bool UInventoryComponent::AddCaughtFish(const FCaughtFishInstance& FishInstance)
{
    if (IsStorageFull())
    {
        OnStorageFull.Broadcast(GetCurrentFishWeightKg());
        return false;
    }

    float FishWeight = FishInstance.WeightKg;
    if (GetCurrentFishWeightKg() + FishWeight > MaxStorageKg)
    {
        OnStorageFull.Broadcast(GetCurrentFishWeightKg());
        return false;
    }

    CaughtFish.Add(FishInstance);
    CachedFishWeight += FishWeight;
    OnInventoryChanged.Broadcast();
    return true;
}

void UInventoryComponent::RemoveAllFish()
{
    CaughtFish.Empty();
    CachedFishWeight = 0.f;
    OnInventoryChanged.Broadcast();
}

bool UInventoryComponent::AddItem(FName ItemID, int32 Quantity)
{
    for (FInventoryItem& Item : Items)
    {
        if (Item.ItemID == ItemID)
        {
            Item.Quantity += Quantity;
            OnInventoryChanged.Broadcast();
            return true;
        }
    }

    FInventoryItem NewItem;
    NewItem.ItemID = ItemID;
    NewItem.Quantity = Quantity;
    Items.Add(NewItem);
    OnInventoryChanged.Broadcast();
    return true;
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
    for (int32 i = 0; i < Items.Num(); i++)
    {
        if (Items[i].ItemID == ItemID)
        {
            if (Items[i].Quantity < Quantity)
                return false;

            Items[i].Quantity -= Quantity;
            if (Items[i].Quantity <= 0)
            {
                Items.RemoveAt(i);
            }
            OnInventoryChanged.Broadcast();
            return true;
        }
    }
    return false;
}

bool UInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
    return GetItemCount(ItemID) >= Quantity;
}

int32 UInventoryComponent::GetItemCount(FName ItemID) const
{
    for (const FInventoryItem& Item : Items)
    {
        if (Item.ItemID == ItemID)
        {
            return Item.Quantity;
        }
    }
    return 0;
}

float UInventoryComponent::GetCurrentFishWeightKg() const
{
    return CachedFishWeight;
}

bool UInventoryComponent::IsStorageFull() const
{
    return CachedFishWeight >= MaxStorageKg;
}

int64 UInventoryComponent::GetTotalFishValue() const
{
    int64 Total = 0;
    for (const FCaughtFishInstance& Fish : CaughtFish)
    {
        Total += Fish.EstimatedValue;
    }
    return Total;
}

int64 UInventoryComponent::SellAllFish()
{
    int64 TotalEarned = GetTotalFishValue();

    if (TotalEarned > 0)
    {
        if (UGamekuGameInstance* GI = Cast<UGamekuGameInstance>(
            UGameplayStatics::GetGameInstance(GetOwner())))
        {
            GI->AddMoney(TotalEarned);
        }
    }

    RemoveAllFish();
    return TotalEarned;
}
