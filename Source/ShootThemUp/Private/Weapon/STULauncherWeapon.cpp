// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include <Kismet/GameplayStatics.h>

void ASTULauncherWeapon::StartFire()
{
    MakeShot();
}

void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld() || IsOutOfAmmo()) return;

    FVector CameraTraceStart, CameraTraceEnd;
    if (!GetCameraTraceData(CameraTraceStart, CameraTraceEnd)) return;

    FHitResult HitResult;
    LineTrace(HitResult, CameraTraceStart, CameraTraceEnd);

    const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd;
    const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

    const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
    ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
    if (Projectile)
    {
        Projectile->SetShotDirection(Direction);
        Projectile->SetOwner(GetOwner());
        Projectile->FinishSpawning(SpawnTransform);
    }

    DecreaseAmmo();
}
