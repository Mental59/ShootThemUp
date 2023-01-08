// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STURifleWeapon.h"
#include <Engine/World.h>
#include <DrawDebugHelpers.h>

void ASTURifleWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTURifleWeapon::MakeShot, FireRate, true);
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void ASTURifleWeapon::MakeShot()
{
    FVector CameraTraceStart, CameraTraceEnd;
    if (!GetWorld() || IsOutOfAmmo() || !GetCameraTraceData(CameraTraceStart, CameraTraceEnd))
    {
        StopFire();
        return;
    }

    FHitResult HitResult;
    LineTrace(HitResult, CameraTraceStart, CameraTraceEnd);

    MakeDamageToActor(HitResult);
    DrawDebugGeometry(HitResult);

    DecreaseAmmo();
}

bool ASTURifleWeapon::GetCameraTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    const APlayerController* Controller = GetPlayerController();
    if (!Controller) return false;

    FVector ViewLocation;
    FRotator ViewRotation;
    Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);

    TraceStart = ViewLocation;
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(FireSpread));
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

void ASTURifleWeapon::MakeDamageToActor(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        AActor* DamagedActor = HitResult.GetActor();
        if (!DamagedActor) return;
        DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
    }
}
