// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUSpectatorWidget.h"
#include "Components/STURespawnComponent.h"
#include "GameFramework/PlayerController.h"

bool USTUSpectatorWidget::GetRespawnTime(float& SecondsLeft) const
{
    if (APlayerController* PlayerController = GetOwningPlayer())
    {
        if (USTURespawnComponent* RespawnComponent = PlayerController->FindComponentByClass<USTURespawnComponent>())
        {
            SecondsLeft = RespawnComponent->GetRespawnSecondsLeft();
            return RespawnComponent->IsRespawnInProgress();
        }
    }
    return false;
}
