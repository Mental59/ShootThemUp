// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUGameOverWidget.h"
#include "STUGameModeBase.h"
#include "Player/STUPlayerState.h"
#include "UI/STUPlayerStatsRowWidget.h"
#include "Components/VerticalBox.h"
#include "STUUtils.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USTUGameOverWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    if (UWorld* World = GetWorld())
    {
        if (ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(World->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &USTUGameOverWidget::OnMatchStateChanged);
        }
    }

    if (RestartLevelButton)
    {
        RestartLevelButton->OnClicked.AddDynamic(this, &USTUGameOverWidget::OnRestartLevel);
    }
}

void USTUGameOverWidget::OnMatchStateChanged(ESTUMatchState MatchState)
{
    if (MatchState == ESTUMatchState::GameOver)
    {
        UpdatePlayersStats();
    }
}

void USTUGameOverWidget::UpdatePlayersStats()
{
    if (!GetWorld() || !PlayerStatsBox) return;

    PlayerStatsBox->ClearChildren();

    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; It++)
    {
        if (!It->IsValid()) continue;

        const AController* Controller = It->Get();

        const ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
        if (!PlayerState) continue;

        USTUPlayerStatsRowWidget* PlayerStatRowWidget = CreateWidget<USTUPlayerStatsRowWidget>(GetWorld(), PlayerStatsRowWidgetClass);
        if (!PlayerStatRowWidget) continue;

        FPlayerStats PlayerStats = PlayerState->GetPlayerStats();

        PlayerStatRowWidget->SetPlayerName(FText::FromString(PlayerState->GetPlayerName()));
        PlayerStatRowWidget->SetKills(STUUtils::TextFromInt(PlayerStats.Kills));
        PlayerStatRowWidget->SetDeaths(STUUtils::TextFromInt(PlayerStats.Deaths));
        PlayerStatRowWidget->SetTeam(STUUtils::TextFromInt(PlayerState->GetTeamID()));
        PlayerStatRowWidget->SetPlayerIndicatorVisibility(Controller->IsPlayerController());

        PlayerStatsBox->AddChild(PlayerStatRowWidget);
    }
}

void USTUGameOverWidget::OnRestartLevel()
{
    const FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this);
    UGameplayStatics::OpenLevel(this, FName(CurrentLevelName));
}
