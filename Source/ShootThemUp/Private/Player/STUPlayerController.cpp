// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUPlayerController.h"
#include "Components/STURespawnComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "STUGameModeBase.h"
#include "STUGameInstance.h"

ASTUPlayerController::ASTUPlayerController()
{
    RespawnComponent = CreateDefaultSubobject<USTURespawnComponent>("RespawnComponent");
}

void ASTUPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->AddMappingContext(MappingContext, 1);
    }

    if (UWorld* World = GetWorld())
    {
        if (ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(World->GetAuthGameMode()))
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUPlayerController::OnMatchStateChanged);
        }
    }
}

void ASTUPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputComponent) return;

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent.Get()))
    {
        if (PauseAction) PauseAction->bTriggerWhenPaused = true;
        EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ASTUPlayerController::OnGamePause);
        EnhancedInputComponent->BindAction(MuteAction, ETriggerEvent::Triggered, this, &ASTUPlayerController::OnMuteSound);
    }
}

void ASTUPlayerController::OnGamePause()
{
    UE_LOG(LogTemp, Warning, TEXT("OnGamePause"));
    if (GetWorld() && GetWorld()->GetAuthGameMode())
    {
        GetWorld()->GetAuthGameMode()->SetPause(this);
    }
}

void ASTUPlayerController::OnMatchStateChanged(ESTUMatchState MatchState)
{
    CurrentMatchState = MatchState;
    if (MatchState == ESTUMatchState::InProgress)
    {
        SetInputMode(FInputModeGameOnly());
        bShowMouseCursor = false;
    }
    else
    {
        SetInputMode(FInputModeUIOnly());
        bShowMouseCursor = true;
    }
}

void ASTUPlayerController::OnMuteSound()
{
    if (!GetWorld()) return;
    
    if (const USTUGameInstance* STUGameInstance = GetWorld()->GetGameInstance<USTUGameInstance>())
    {
        STUGameInstance->ToggleVolume();
    }
}
