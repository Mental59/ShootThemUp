// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUProjectile.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUProjectile : public AActor
{
    GENERATED_BODY()

public:
    ASTUProjectile();

protected:
    UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
    class USphereComponent* CollisionComponent;

    virtual void BeginPlay() override;
};
