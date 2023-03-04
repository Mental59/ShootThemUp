// Shoot Them Up Game, All Rights Reserved.

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameModeBase, All, All);

ASTUGameModeBase::ASTUGameModeBase()
{
    DefaultPawnClass = ASTUBaseCharacter::StaticClass();
    PlayerControllerClass = ASTUPlayerController::StaticClass();
    HUDClass = ASTUGameHUD::StaticClass();
    PlayerStateClass = ASTUPlayerState::StaticClass();
}

void ASTUGameModeBase::StartPlay()
{
    Super::StartPlay();

    SpawnBots();
    CreateTeamsInfo();

    CurrentRound = 1;
    StartRound();
}

UClass* ASTUGameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
    if (InController && InController->IsA<AAIController>())
    {
        return AIPawnClass;
    }
    return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ASTUGameModeBase::Killed(AController* KillerController, AController* VictimController)
{
    ASTUPlayerState* KillerPlayerState = KillerController ? Cast<ASTUPlayerState>(KillerController->PlayerState) : nullptr;
    ASTUPlayerState* VictimPlayerState = VictimController ? Cast<ASTUPlayerState>(VictimController->PlayerState) : nullptr;

    if (KillerPlayerState)
    {
        KillerPlayerState->AddKill();
    }

    if (VictimPlayerState)
    {
        VictimPlayerState->AddDeath();
    }
}

void ASTUGameModeBase::SpawnBots()
{
    if (!GetWorld()) return;

    for (int32 i = 0; i < GameData.PlayersNum - 1; i++)
    {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        AAIController* const STUAIController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);
        RestartPlayer(STUAIController);
    }
}

void ASTUGameModeBase::StartRound()
{
    RoundCountDown = GameData.RoundTimeSec;
    GetWorldTimerManager().SetTimer(GameRoundTimerHandle, this, &ASTUGameModeBase::UpdateGameTimer, 1.0f, true);
}

void ASTUGameModeBase::UpdateGameTimer()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("Time: %i; Round: %i/%i"), RoundCountDown, CurrentRound, GameData.RoundsNum);

    if (--RoundCountDown == 0)
    {
        GetWorldTimerManager().ClearTimer(GameRoundTimerHandle);

        if (CurrentRound + 1 <= GameData.RoundsNum)
        {
            CurrentRound++;
            ResetPlayers();
            StartRound();
        }
        else
        {
            UE_LOG(LogSTUGameModeBase, Display, TEXT("----- GAME OVER -----"));
            LogPlayerInfo();
        }
    }
}

void ASTUGameModeBase::ResetPlayers()
{
    if (!GetWorld()) return;

    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; It++)
    {
        if (It->IsValid()) ResetOnePlayer(It->Get());
    }
}

void ASTUGameModeBase::ResetOnePlayer(AController* InController)
{
    if (InController && InController->GetPawn())
    {
        InController->GetPawn()->Reset();
    }
    RestartPlayer(InController);
    SetPlayerColor(InController);
}

void ASTUGameModeBase::CreateTeamsInfo()
{
    if (!GetWorld()) return;

    int32 TeamID = 1;
    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; It++)
    {
        if (!It->IsValid()) continue;

        AController* Controller = It->Get();
        ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);

        PlayerState->SetTeamID(TeamID);
        PlayerState->SetTeamColor(GetColorByTeamID(TeamID));
        SetPlayerColor(Controller);

        TeamID = TeamID == 1 ? 2 : 1;
    }
}

const FLinearColor& ASTUGameModeBase::GetColorByTeamID(int32 TeamID) const
{
    if (TeamID - 1 < GameData.TeamColors.Num())
    {
        return GameData.TeamColors[TeamID - 1];
    }

    UE_LOG(
        LogSTUGameModeBase, Warning, TEXT("No Color for team id: %i, set to default: %s"), TeamID, *GameData.DefaultTeamColor.ToString());

    return GameData.DefaultTeamColor;
}

void ASTUGameModeBase::SetPlayerColor(AController* Controller)
{
    if (!Controller) return;

    ASTUBaseCharacter* Character = Cast<ASTUBaseCharacter>(Controller->GetPawn());
    ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(Controller->PlayerState);
    if (!Character || !PlayerState) return;

    Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTUGameModeBase::LogPlayerInfo()
{
    for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; It++)
    {
        if (!It->IsValid()) continue;

        if (ASTUPlayerState* PlayerState = Cast<ASTUPlayerState>(It->Get()->PlayerState))
        {
            PlayerState->LogInfo();
        }
    }
}