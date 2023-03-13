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
    virtual void Tick(float DeltaTime) override;
    bool CouldBeTaken() const;

protected:
    UPROPERTY(VisibleAnywhere, Category = "Pickup")
    class USphereComponent* CollisionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float RespawnTime = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
    float CollisionEnadleTime = 0.2f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    class USoundCue* PickupSound;

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

    virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

    virtual void BeginPlay() override;

    virtual bool GivePickupTo(APawn* PlayerPawn);

    void PlayPickupSound() const;

private:
    UPROPERTY()
    TArray<APawn*> OverlappingPawns;

    FTimerHandle RespawnTimerHandle;
    float RotationYaw = 0.0f;

    void PickupWasTaken();
    void Respawn();

    void GenerateRotationYaw();

    void EnableCollision();
};
