// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUGameHUD.h"
#include <Engine/Canvas.h>
#include <Blueprint/UserWidget.h>

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    DrawCrosshair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    if (UUserWidget* PlayerHUDWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass))
    {
        PlayerHUDWidget->AddToViewport();
    }
}

void ASTUGameHUD::DrawCrosshair()
{
    const float CenterX = Canvas->SizeX * 0.5f;
    const float CenterY = Canvas->SizeY * 0.5f;

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;

    DrawLine(CenterX - HalfLineSize, CenterY, CenterX + HalfLineSize, CenterY, LineColor, LineThickness);
    DrawLine(CenterX, CenterY - HalfLineSize, CenterX, CenterY + HalfLineSize, LineColor, LineThickness);
}
