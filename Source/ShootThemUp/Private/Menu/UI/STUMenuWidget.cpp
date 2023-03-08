// Shoot Them Up Game, All Rights Reserved.

#include "Menu/UI/STUMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "STUGameInstance.h"

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

void USTUMenuWidget::OnStartGame()
{
    if (!GetWorld()) return;

    if (const USTUGameInstance* GameInstance = GetWorld()->GetGameInstance<USTUGameInstance>())
    {
        UGameplayStatics::OpenLevel(this, GameInstance->GetStartUpLevel().LevelName);
    }
}

void USTUMenuWidget::OnExitGame()
{
    UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}