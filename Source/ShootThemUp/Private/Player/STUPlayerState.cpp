// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUPlayerState.h"

void ASTUPlayerState::SetTeamID(int32 ID)
{
    TeamID = ID;
}

int32 ASTUPlayerState::GetTeamID() const
{
    return TeamID;
}

void ASTUPlayerState::SetTeamColor(const FLinearColor& Color)
{
    TeamColor = Color;
}

const FLinearColor& ASTUPlayerState::GetTeamColor() const
{
    return TeamColor;
}
