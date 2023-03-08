// Shoot Them Up Game, All Rights Reserved.

#include "Menu/UI/STUMenuHUD.h"
#include "UI/STUBaseWidget.h"

void ASTUMenuHUD::BeginPlay()
{
    Super::BeginPlay();

    if (USTUBaseWidget* MenuWidget = CreateWidget<USTUBaseWidget>(GetWorld(), MenuWidgetClass))
    {
        MenuWidget->AddToViewport();
        MenuWidget->Show();
    }
}
