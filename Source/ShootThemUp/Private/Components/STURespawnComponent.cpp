// Shoot Them Up Game, All Rights Reserved.

#include "Components/STURespawnComponent.h"
#include "STUGameModeBase.h"

USTURespawnComponent::USTURespawnComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTURespawnComponent::Respawn(float RespawnTimeSeconds)
{
    if (!GetWorld()) return;

    RespawnSecondsLeft = RespawnTimeSeconds;
    GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &USTURespawnComponent::RespawnTimerUpdate, TimerTick, true);
}

float USTURespawnComponent::GetRespawnSecondsLeft() const
{
    return RespawnSecondsLeft;
}

bool USTURespawnComponent::IsRespawnInProgress() const
{
    return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimerHandle);
}

void USTURespawnComponent::RespawnTimerUpdate()
{
    RespawnSecondsLeft -= TimerTick;
    if (GetWorld() && RespawnSecondsLeft <= 0.0f)
    {
        GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
        if (ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
        {
            GameMode->RequestPlayerRespawn(Cast<AController>(GetOwner()));
        }
    }
}
