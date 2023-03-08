// Shoot Them Up Game, All Rights Reserved.

#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Image.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuWidget, All, All);

void USTUMenuWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnStartGame);
    }

    if (ExitGameButton)
    {
        ExitGameButton->OnClicked.AddDynamic(this, &USTUMenuWidget::OnExitGame);
    }
}

void USTUMenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
    if (!GetWorld() || Animation != HideAnimation) return;

    if (const USTUGameInstance* GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>())
    {
        UGameplayStatics::OpenLevel(this, GameInstance->GetStartUpLevel().LevelName);
    }
}

void USTUMenuWidget::OnStartGame()
{
    if (ShowImage)
    {
        ShowImage->SetVisibility(ESlateVisibility::Visible);
    }
    
    PlayAnimation(HideAnimation);
}

void USTUMenuWidget::OnExitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}
