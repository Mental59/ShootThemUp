// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include <Kismet/GameplayStatics.h>
#include "Sound/SoundCue.h"
#include "Animations/AnimUtils.h"
#include "Animations/STULauncherReloadStartAnimNotify.h"
#include "Animations/STULauncherReloadEndAnimNotify.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogLauncherWeapon, All, All);

void ASTULauncherWeapon::StartFire()
{
    if (CanShoot) MakeShot();
}

void ASTULauncherWeapon::SetAnimNotifications(UAnimMontage* ReloadAnimMontage)
{
    Super::SetAnimNotifications(ReloadAnimMontage);
    
    if (auto ReloadStartNotify = AnimUtils::FindFirstNotifyByClass<USTULauncherReloadStartAnimNotify>(ReloadAnimMontage))
    {
        ReloadStartNotify->OnNotified.AddUObject(this, &ASTULauncherWeapon::OnReloadStart);
    }
    else
    {
        UE_LOG(LogLauncherWeapon, Error, TEXT("Laucnher reload start anim notify is not set"));
        checkNoEntry();
    }

    if (auto ReloadEndNotify = AnimUtils::FindFirstNotifyByClass<USTULauncherReloadEndAnimNotify>(ReloadAnimMontage))
    {
        ReloadEndNotify->OnNotified.AddUObject(this, &ASTULauncherWeapon::OnReloadEnd);
    }
    else
    {
        UE_LOG(LogLauncherWeapon, Error, TEXT("Launcher reload end anim notify is not set"));
        checkNoEntry();
    }
}

void ASTULauncherWeapon::StopReloadSounds()
{
    Super::StopReloadSounds();
    if (ReloadEndAudioComponent)
    {
        ReloadEndAudioComponent->Stop();
    }
}

void ASTULauncherWeapon::MakeShot()
{
    if (!GetWorld()) return;

    if (IsOutOfAmmo())
    {
        UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
        return;
    }

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
    SpawnMuzzleFX();

    CanShoot = false;
    GetWorld()->GetTimerManager().SetTimer(ShootTimer, this, &ASTULauncherWeapon::ActivateShooting, FireRate, false);
    UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
}

void ASTULauncherWeapon::OnReloadEnd(USkeletalMeshComponent* MeshComp)
{
    if (const ACharacter* Character = GetPlayer())
    {
        if (Character->GetMesh() != MeshComp) return;
        ReloadEndAudioComponent = UGameplayStatics::SpawnSoundAttached(ReloadEndSound, WeaponMesh);
    }
}

void ASTULauncherWeapon::ActivateShooting()
{
    CanShoot = true;
}
