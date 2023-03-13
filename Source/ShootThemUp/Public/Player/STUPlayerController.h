// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "STUCoreTypes.h"
#include "InputMappingContext.h"
#include "STUPlayerController.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ASTUPlayerController();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    class USTURespawnComponent* RespawnComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* MappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* PauseAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* MuteAction;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    ESTUMatchState CurrentMatchState = ESTUMatchState::WaitingToStart;

    void OnGamePause();
    void OnMatchStateChanged(ESTUMatchState MatchState);
    void OnMuteSound();
};
