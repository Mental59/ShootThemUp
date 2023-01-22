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

    UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
    void OnHit();

    void SetShotDirection(const FVector& Direction);

protected:
    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    class USphereComponent* CollisionComponent;

    UPROPERTY(VisibleAnywhere, Category = "Weapon")
    class UProjectileMovementComponent* MovementComponent;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    class USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = "0.0"))
    float LifeSpan = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", meta = (ClampMin = "0.0"))
    float LifeSpanAfterHit = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float DamageRaduis = 300.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
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
