// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUPlayerStatsRowWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void USTUPlayerStatsRowWidget::SetPlayerName(const FText& Text)
{
    if (!PlayerNameTextBlock) return;
    PlayerNameTextBlock->SetText(Text);
}

void USTUPlayerStatsRowWidget::SetKills(const FText& Text)
{
    if (!KillsTextBlock) return;
    KillsTextBlock->SetText(Text);
}

void USTUPlayerStatsRowWidget::SetDeaths(const FText& Text)
{
    if (!DeathsTextBlock) return;
    DeathsTextBlock->SetText(Text);
}

void USTUPlayerStatsRowWidget::SetTeam(const FText& Text)
{
    if (!TeamTextBlock) return;
    TeamTextBlock->SetText(Text);
}

void USTUPlayerStatsRowWidget::SetPlayerIndicatorVisibility(bool Visible)
{
    if (!PlayerIndicatorImage) return;
    PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
