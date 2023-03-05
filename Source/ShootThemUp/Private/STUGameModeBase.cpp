// Shoot Them Up Game, All Rights Reserved.

#include "STUGameModeBase.h"
#include "Player/STUBaseCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"
#include "GameFramework/Controller.h"
#include "AIController.h"
#include "Player/STUPlayerState.h"
#include "Components/STURespawnComponent.h"
#include "EngineUtils.h"

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

    SetMatchState(ESTUMatchState::InProgress);
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

    StartRespawn(VictimController);
}

FRoundInfo ASTUGameModeBase::GetRoundInfo() const
{
    FRoundInfo RoundInfo;

    RoundInfo.RoundsNum = GameData.RoundsNum;
    RoundInfo.CurrentRoundNum = CurrentRound;
    RoundInfo.SecondsLeft = RoundCountDown;

    return RoundInfo;
}

void ASTUGameModeBase::RequestPlayerRespawn(AController* PlayerController)
{
    ResetOnePlayer(PlayerController);
}

bool ASTUGameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
    bool IsPauseSet = Super::SetPause(PC, CanUnpauseDelegate);
    if (IsPauseSet)
    {
        SetMatchState(ESTUMatchState::Pause);
    }
    return IsPauseSet;
}

bool ASTUGameModeBase::ClearPause()
{
    bool IsPauseCleared = Super::ClearPause();
    if (IsPauseCleared)
    {
        SetMatchState(ESTUMatchState::InProgress);
    }
    return IsPauseCleared;
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
            OnGameOver();
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
        PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
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

void ASTUGameModeBase::StartRespawn(AController* PlayerController)
{
    bool IsRespawnAvailable = (float)RoundCountDown > (float)MinRoundTimeForRespawn + GameData.RespawnTimeSeconds;
    if (!IsRespawnAvailable) return;

    if (USTURespawnComponent* RespawnComponent = PlayerController->FindComponentByClass<USTURespawnComponent>())
    {
        RespawnComponent->Respawn(GameData.RespawnTimeSeconds);
    }
}

void ASTUGameModeBase::OnGameOver()
{
    UE_LOG(LogSTUGameModeBase, Display, TEXT("----- GAME OVER -----"));
    LogPlayerInfo();

    for (APawn* Pawn : TActorRange<APawn>(GetWorld()))
    {
        if (Pawn)
        {
            Pawn->TurnOff();
            Pawn->DisableInput(nullptr);
        }
    }

    SetMatchState(ESTUMatchState::GameOver);
}

void ASTUGameModeBase::SetMatchState(ESTUMatchState State)
{
    if (MatchState == State) return;

    MatchState = State;
    OnMatchStateChanged.Broadcast(MatchState);
}
