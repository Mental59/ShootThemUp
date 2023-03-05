// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STUGameOverWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUGameOverWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* PlayerStatsBox;

    UPROPERTY(meta = (BindWidget))
    class UButton* RestartLevelButton;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<UUserWidget> PlayerStatsRowWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    void OnMatchStateChanged(ESTUMatchState MatchState);
    void UpdatePlayersStats();

    UFUNCTION()
    void OnRestartLevel();
};
