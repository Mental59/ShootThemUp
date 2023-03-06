// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUToMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUToMenuWidget, All, All);

void USTUToMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (ToMenuButton)
    {
        ToMenuButton->OnClicked.AddDynamic(this, &USTUToMenuWidget::OnToMenu);
    }
}

void USTUToMenuWidget::OnToMenu()
{
    if (!GetWorld()) return;

    if (const USTUGameInstance* GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>())
    {
        if (GameInstance->GetMenuLevelName().IsNone())
        {
            UE_LOG(LogSTUToMenuWidget, Error, TEXT("MenuLevelName is None"));
        }
        else
        {
            UGameplayStatics::OpenLevel(this, GameInstance->GetMenuLevelName());
        }
    }
}
