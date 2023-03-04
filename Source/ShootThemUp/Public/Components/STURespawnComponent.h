// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STURespawnComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTURespawnComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTURespawnComponent();

    void Respawn(float RespawnTimeSeconds);
    float GetRespawnSecondsLeft() const;
    bool IsRespawnInProgress() const;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timer", meta = (ClampMin = "0"))
    float TimerTick = 0.1f;

private:
    FTimerHandle RespawnTimerHandle;
    float RespawnSecondsLeft;

    void RespawnTimerUpdate();
};
