// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBasePickup : public AActor
{
    GENERATED_BODY()

public:
    ASTUBasePickup();

protected:
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    class USphereComponent* CollisionComponent;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
};
