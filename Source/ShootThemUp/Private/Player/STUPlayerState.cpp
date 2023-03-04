// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUPlayerState, All, All);

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

void ASTUPlayerState::AddKill()
{
    KillsNum++;
}

void ASTUPlayerState::AddDeath()
{
    DeathsNum++;
}

int32 ASTUPlayerState::GetKillsNum()
{
    return KillsNum;
}

int32 ASTUPlayerState::GetDeathsNum()
{
    return DeathsNum;
}

void ASTUPlayerState::LogInfo() const
{
    UE_LOG(LogSTUPlayerState, Display, TEXT("TeamID: %i, Kills: %i, Deaths: %i"), TeamID, KillsNum, DeathsNum);
}
