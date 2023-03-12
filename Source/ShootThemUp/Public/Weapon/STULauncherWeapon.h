// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STULauncherWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTULauncherWeapon : public ASTUBaseWeapon
{
    GENERATED_BODY()

public:
    virtual void StartFire() override;
    virtual void SetAnimNotifications(class UAnimMontage* ReloadAnimMontage) override;
    virtual void StopReloadSounds() override;

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<class ASTUProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0"))
    float FireRate = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    class USoundCue* ReloadEndSound;

    UPROPERTY(BlueprintReadWrite, Category = "Sound")
    class UAudioComponent* ReloadEndAudioComponent = nullptr;

    virtual void MakeShot() override;

    void OnReloadEnd(USkeletalMeshComponent* MeshComp);

private:
    FTimerHandle ShootTimer;
    bool CanShoot = true;

    void ActivateShooting();
};
