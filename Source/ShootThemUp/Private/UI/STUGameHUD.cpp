// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUGameHUD.h"
#include <Engine/Canvas.h>
#include <Blueprint/UserWidget.h>
#include "STUGameModeBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    // DrawCrosshair();
}

void ASTUGameHUD::BeginPlay()
{
    Super::BeginPlay();

    GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<UUserWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::Pause, CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<UUserWidget>(GetWorld(), GameOverWidgetClass));

    for (const auto& GameWidgetPair : GameWidgets)
    {
        if (GameWidgetPair.Value)
        {
            GameWidgetPair.Value->AddToViewport();
            GameWidgetPair.Value->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (UWorld* World = GetWorld())
    {
        if (ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(World->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
        }
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

void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState MatchState)
{
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (GameWidgets.Contains(MatchState))
    {
        CurrentWidget = GameWidgets[MatchState];
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
    }

    UE_LOG(LogSTUGameHUD, Display, TEXT("Match state has been changed: %s"), *UEnum::GetValueAsString(MatchState));
}
