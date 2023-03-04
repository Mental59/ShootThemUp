// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUGameDataWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUGameDataWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetPlayerStats(FPlayerStats& PlayerStats) const;

    UFUNCTION(BlueprintCallable, Category = "UI")
    bool GetRoundInfo(FRoundInfo& RoundInfo) const;

protected:
    class ASTUGameModeBase* GetSTUGameMode() const;
    class ASTUPlayerState* GetSTUPlayerState() const;
};
