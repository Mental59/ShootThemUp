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
    virtual void SetAnimNotifications(class UAnimMontage* ReloadAnimMontage) override;
    virtual void Zoom(bool Enabled) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float FireRate = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float DefaultFireSpread = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float ZoomFireSpread = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float DamageAmount = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float FOVZoomAngle = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    class UNiagaraSystem* TraceFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FX")
    FString TraceTargetName = "TraceTarget";

    UPROPERTY(VisibleAnywhere, Category = "VFX")
    class USTUWeaponFXComponent* WeaponFXComponent;

    UPROPERTY(BlueprintReadWrite, Category = "Sound")
    class UAudioComponent* FireAudioComponent;
    
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void MakeShot() override;
    virtual bool GetCameraTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
    void MakeDamageToActor(const struct FHitResult& HitResult);

private:
    FTimerHandle ShootTimerHandle;
    float DefaultCameraFOV;
    float CurrentFireSpread;

    UPROPERTY()
    class UNiagaraComponent* MuzzleFXComponent;

    void PlayImpactFX(const struct FHitResult& HitResult);
    void InitFX();
    void SetFXActive(bool IsActive);
    void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
    void PlayAllFX(const struct FHitResult& HitResult);
    class APlayerController* GetPlayerController() const;
};
