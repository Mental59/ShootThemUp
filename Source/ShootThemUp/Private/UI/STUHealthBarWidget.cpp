// Shoot Them Up Game, All Rights Reserved.


#include "UI/STUHealthBarWidget.h"
#include "Components/ProgressBar.h"

void USTUHealthBarWidget::SetHealthPercent(float HealthPercent)
{
    if (HealthProgressBar)
    {
        HealthProgressBar->SetFillColorAndOpacity(HealthPercent > PercentColorChangeThreshold ? DefaultColor : LowHealthColor);
        HealthProgressBar->SetPercent(HealthPercent);
    }
}

void USTUHealthBarWidget::UpdateHealthBarTimer(bool Visible)
{
    if (GetWorld() && HealthProgressBar && Visible)
    {
        HealthProgressBar->SetVisibility(ESlateVisibility::Visible);
        GetWorld()->GetTimerManager().SetTimer(
            HealthBarTimerHandle, this, &USTUHealthBarWidget::HideHealthBar, HideBarDelay, false);
    }
}

void USTUHealthBarWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (HealthProgressBar) HideHealthBar();
}

void USTUHealthBarWidget::HideHealthBar()
{
    HealthProgressBar->SetVisibility(ESlateVisibility::Hidden);
}
