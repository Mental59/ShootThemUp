// Shoot Them Up Game, All Rights Reserved.

#include "Menu/UI/STUMenuHUD.h"
#include "Blueprint/UserWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (UUserWidget* MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass))
    {
        MenuWidget->AddToViewport();
    }
}
