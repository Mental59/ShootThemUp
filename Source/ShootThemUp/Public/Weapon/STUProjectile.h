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

    void SetShotDirection(const FVector& Direction);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    class UProjectileMovementComponent* MovementComponent;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    class USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    float LifeSpan = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageRaduis = 200.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool DoFullDamage = false;

    virtual void BeginPlay() override;

private:
    FVector ShotDirection;

    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
        const FHitResult& Hit);

    AController* GetOwnerController() const;
};
