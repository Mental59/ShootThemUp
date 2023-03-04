// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUGameDataWidget.h"
#include "Player/STUPlayerState.h"
#include "STUGameModeBase.h"

bool USTUGameDataWidget::GetPlayerStats(FPlayerStats& PlayerStats) const
{
    if (ASTUPlayerState* PlayerState = GetSTUPlayerState())
    {
        PlayerStats = PlayerState->GetPlayerStats();
        return true;
    }

    return false;
}

bool USTUGameDataWidget::GetRoundInfo(FRoundInfo& RoundInfo) const
{
    if (ASTUGameModeBase* GameMode = GetSTUGameMode())
    {
        RoundInfo = GameMode->GetRoundInfo();
        return true;
    }
    return false;
}

ASTUGameModeBase* USTUGameDataWidget::GetSTUGameMode() const
{
    return GetWorld() ? Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASTUPlayerState* USTUGameDataWidget::GetSTUPlayerState() const
{
    return GetOwningPlayerPawn() ? Cast<ASTUPlayerState>(GetOwningPlayer()->PlayerState) : nullptr;
}
