// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUPlayerStatsRowWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUPlayerStatsRowWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    void SetPlayerName(const class FText& Text);
    void SetKills(const class FText& Text);
    void SetDeaths(const class FText& Text);
    void SetTeam(const class FText& Text);
    void SetPlayerIndicatorVisibility(bool Visible);

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* PlayerNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* KillsTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* DeathsTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TeamTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UImage* PlayerIndicatorImage;
};
