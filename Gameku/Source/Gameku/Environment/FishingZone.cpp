#include "Environment/FishingZone.h"
#include "Fishing/FishingSpot.h"

UFishingZone::UFishingZone()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFishingZone::OnHourChanged(float NewHour)
{
    for (auto& SpotPtr : RegisteredSpots)
    {
        if (SpotPtr.IsValid())
        {
            SpotPtr->OnHourChanged(NewHour);
        }
    }
}

void UFishingZone::RegisterSpot(AFishingSpot* Spot)
{
    if (Spot)
    {
        RegisteredSpots.AddUnique(Spot);
    }
}

void UFishingZone::UnlockZone()
{
    bIsUnlocked = true;
    UE_LOG(LogTemp, Log, TEXT("Zone dibuka: %s"), *ZoneName.ToString());
}
