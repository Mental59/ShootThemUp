// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <Engine/World.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
    check(WeaponMesh);
}

void ASTUBaseWeapon::StartFire()
{
    MakeShot();
    GetWorldTimerManager().SetTimer(ShootTimerHandle, this, &ASTUBaseWeapon::MakeShot, FireRate, true);
}

void ASTUBaseWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShootTimerHandle);
}

void ASTUBaseWeapon::MakeShot()
{
    if (!GetWorld()) return;

    FVector CameraTraceStart, CameraTraceEnd;
    if (!GetCameraTraceData(CameraTraceStart, CameraTraceEnd)) return;

    FHitResult HitResult;
    LineTrace(HitResult, CameraTraceStart, CameraTraceEnd);

    MakeDamageToActor(HitResult);
    DrawDebugGeometry(HitResult);
}

void ASTUBaseWeapon::LineTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());

    const ACharacter* Player = GetPlayer();

    TArray<FHitResult> HitResults;
    int32 Index = -1;
    GetWorld()->LineTraceMultiByChannel(HitResults, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
    for (int32 i = 0; i < HitResults.Num(); i++)
    {
        if (GetAngleBetweenPlayerAndTrace(HitResults[i].ImpactPoint, Player) <= MaxAngleBetweenPlayerAndTrace)
        {
            Index = i;
            break;
        }
    }

    const FVector MuzzleLocation = GetMuzzleWorldLocation();
    const FVector TraceFinalDirection = ((Index >= 0 ? HitResults[Index].ImpactPoint : TraceEnd) - MuzzleLocation).GetSafeNormal();
    GetWorld()->LineTraceSingleByChannel(HitResult, MuzzleLocation, MuzzleLocation + TraceFinalDirection * TraceMaxDistance,
        ECollisionChannel::ECC_Visibility, CollisionParams);
}

bool ASTUBaseWeapon::GetCameraTraceData(FVector& TraceStart, FVector& TraceEnd) const
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

float ASTUBaseWeapon::GetAngleBetweenPlayerAndTrace(const FVector& ImpactPoint, const ACharacter* Player) const
{
    const FVector PlayerForwardVector = Player->GetActorRotation().Vector();
    const FVector LineTraceVector = (ImpactPoint - Player->GetActorLocation()).GetSafeNormal();
    const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(PlayerForwardVector, LineTraceVector)));
    return Angle;
}

void ASTUBaseWeapon::DrawDebugGeometry(const FHitResult& HitResult) const
{
    DrawDebugLine(GetWorld(), GetMuzzleWorldLocation(), HitResult.bBlockingHit ? HitResult.ImpactPoint : HitResult.TraceEnd, FColor::Red,
        false, 3.0f, 0, 3.0f);
    if (HitResult.bBlockingHit) DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.0f, 24, FColor::Red, false, 5.0f);
}

void ASTUBaseWeapon::MakeDamageToActor(const FHitResult& HitResult)
{
    if (HitResult.bBlockingHit)
    {
        AActor* DamagedActor = HitResult.GetActor();
        if (!DamagedActor) return;
        DamagedActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
    }
}

APlayerController* ASTUBaseWeapon::GetPlayerController() const
{
    ACharacter* Player = GetPlayer();
    if (!Player) return nullptr;

    return Player->GetController<APlayerController>();
}

ACharacter* ASTUBaseWeapon::GetPlayer() const
{
    return Cast<ACharacter>(GetOwner());
}

FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}
