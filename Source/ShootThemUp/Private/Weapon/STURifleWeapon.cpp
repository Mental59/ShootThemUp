// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STURifleWeapon.h"
#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Animations/AnimUtils.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Animations/STURifleReloadAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogRifleWeapon, All, All);

ASTURifleWeapon::ASTURifleWeapon()
{
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::StartFire()
{
    InitFX();
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTURifleWeapon::MakeShot, FireRate, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShootTimerHandle);
    SetFXActive(false);
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

    check(WeaponFXComponent);

    const APlayerController* Controller = GetPlayerController();
    if (Controller && Controller->PlayerCameraManager)
    {
        DefaultCameraFOV = Controller->PlayerCameraManager->GetFOVAngle();
    }
    CurrentFireSpread = DefaultFireSpread;
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
    PlayAllFX(HitResult);

    DecreaseAmmo();
}

void ASTURifleWeapon::PlayAllFX(const FHitResult& HitResult)
{
    PlayImpactFX(HitResult);
    SpawnTraceFX(GetMuzzleWorldLocation(), HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd);
}

APlayerController* ASTURifleWeapon::GetPlayerController() const
{
    APawn* Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController<APlayerController>() : nullptr;
}

bool ASTURifleWeapon::GetCameraTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
    FVector ViewLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

    TraceStart = ViewLocation;
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), FMath::DegreesToRadians(CurrentFireSpread));
    TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;

    return true;
}

void ASTURifleWeapon::SetAnimNotifications(UAnimMontage* ReloadAnimMontage)
{
    Super::SetAnimNotifications(ReloadAnimMontage);
    if (auto RifleReloadNotify = AnimUtils::FindFirstNotifyByClass<USTURifleReloadAnimNotify>(ReloadAnimMontage))
    {
        RifleReloadNotify->OnNotified.AddUObject(this, &ASTURifleWeapon::OnReloadStart);
    }
    else
    {
        UE_LOG(LogRifleWeapon, Error, TEXT("Rifle reload anim notify is not set"));
        checkNoEntry();
    }
}

void ASTURifleWeapon::Zoom(bool Enabled)
{
    const APlayerController* Controller = GetPlayerController();
    if (Controller && Controller->PlayerCameraManager)
    {
        Controller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
        CurrentFireSpread = Enabled ? ZoomFireSpread : DefaultFireSpread;
    }
}

void ASTURifleWeapon::MakeDamageToActor(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        AActor* DamagedActor = HitResult.GetActor();
        ACharacter* Player = GetPlayer();
        if (!DamagedActor || !Player) return;
        DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), Player->GetController(), this);
    }
}

void ASTURifleWeapon::PlayImpactFX(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        WeaponFXComponent->PlayImpactFX(HitResult);
    }
}

void ASTURifleWeapon::InitFX()
{
    if (!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
    }

    if (!FireAudioComponent)
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
    }
    
    SetFXActive(true);
}

void ASTURifleWeapon::SetFXActive(bool IsActive)
{
    if (MuzzleFXComponent)
    {
        MuzzleFXComponent->SetPaused(!IsActive);
        MuzzleFXComponent->SetVisibility(IsActive, true);
    }

    if (FireAudioComponent)
    {
        FireAudioComponent->SetPaused(!IsActive);
    }
}

void ASTURifleWeapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
    UNiagaraComponent* TraceFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFX, TraceStart);
    if (TraceFXComponent)
    {
        TraceFXComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
    }
}
