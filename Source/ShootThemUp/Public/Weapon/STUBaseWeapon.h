// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreTypes.h"
#include "STUBaseWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    FOnMagazineEmptySignature OnMagazineEmpty;

    virtual void StartFire();
    virtual void StopFire();

    void ChangeMagazine();
    bool CanReload() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    class USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0"))
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "0.0", ClampMax = "180.0"))
    float MaxAngleBetweenPlayerAndTrace = 96.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    virtual void BeginPlay() override;

    virtual void MakeShot();

    virtual bool GetCameraTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    virtual void DrawDebugGeometry(const FHitResult& HitResult) const;

    void LineTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    float GetAngleBetweenPlayerAndTrace(const FVector& ImpactPoint, const ACharacter* Player) const;

    APlayerController* GetPlayerController() const;

    ACharacter* GetPlayer() const;

    FVector GetMuzzleWorldLocation() const;

    void DecreaseAmmo();

    bool IsOutOfAmmo() const;
    bool IsMagazineEmpty() const;
    bool HasMagazines() const;

    void LogAmmo();

private:
    FAmmoData CurrentAmmo;
};
