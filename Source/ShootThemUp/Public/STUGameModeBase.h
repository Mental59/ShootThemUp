// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

public:
    ASTUGameModeBase();

    virtual void StartPlay() override;
    UClass* GetDefaultPawnClassForController_Implementation(class AController* InController);

    void Killed(class AController* KillerController, class AController* VictimController);

    FRoundInfo GetRoundInfo() const;

    void RequestPlayerRespawn(class AController* PlayerController);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<class AAIController> AIControllerClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TSubclassOf<class APawn> AIPawnClass;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    int32 MinRoundTimeForRespawn = 3;

    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FGameData GameData;

private:
    int32 CurrentRound;
    int32 RoundCountDown;
    FTimerHandle GameRoundTimerHandle;

    void SpawnBots();
    void StartRound();
    void UpdateGameTimer();

    void ResetPlayers();
    void ResetOnePlayer(class AController* InController);

    void CreateTeamsInfo();
    const FLinearColor& GetColorByTeamID(int32 TeamID) const;
    void SetPlayerColor(class AController* Controller);

    void LogPlayerInfo();
    void StartRespawn(class AController* PlayerController);
};
