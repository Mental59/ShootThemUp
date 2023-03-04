// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "STUCoreTypes.h"
#include "STUPlayerState.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerState : public APlayerState
{
    GENERATED_BODY()

public:
    void SetTeamID(int32 ID);
    int32 GetTeamID() const;

    void SetTeamColor(const FLinearColor& Color);
    const FLinearColor& GetTeamColor() const;

    void AddKill();
    void AddDeath();

    const FPlayerStats& GetPlayerStats() const;

    void LogInfo() const;

private:
    int32 TeamID;
    FLinearColor TeamColor;
    FPlayerStats PlayerStats;
};
