// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    int32 NumBullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!IsInfinite"))
    int32 NumMagazines;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool IsInfinite;
};

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    virtual void StartFire();
    virtual void StopFire();

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
    void ChangeMagazine();

    bool IsOutOfAmmo() const;
    bool IsMagazineEmpty() const;

    void LogAmmo();

private:
    FAmmoData CurrentAmmo;
};
