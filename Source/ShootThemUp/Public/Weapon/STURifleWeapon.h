// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    ASTURifleWeapon();

    virtual void StartFire() override;
    virtual void StopFire() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (ClampMin = "0.0"))
    float FireRate = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (ClampMin = "0.0"))
    float FireSpread = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (ClampMin = "0.0"))
    float DamageAmount = 10.0f;

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    class USTUWeaponFXComponent* WeaponFXComponent;

    virtual void BeginPlay() override;
    virtual void MakeShot() override;
    virtual bool GetCameraTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    void MakeDamageToActor(const struct FHitResult& HitResult);

private:
    FTimerHandle ShootTimerHandle;

    void PlayImpactFX(const struct FHitResult& HitResult);
};
