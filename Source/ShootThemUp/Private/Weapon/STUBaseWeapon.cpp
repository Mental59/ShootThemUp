// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/STUBaseWeapon.h"
#include <Components/SkeletalMeshComponent.h>
#include <Engine/World.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/Character.h>
#include <GameFramework/Controller.h>
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

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

    CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire() {}

void ASTUBaseWeapon::StopFire() {}

void ASTUBaseWeapon::MakeShot() {}

void ASTUBaseWeapon::LineTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.bReturnPhysicalMaterial = true;

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
    const FVector ShootDirection = ViewRotation.Vector();
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

void ASTUBaseWeapon::DecreaseAmmo()
{
    if (CurrentAmmo.NumBullets == 0) return;

    CurrentAmmo.NumBullets--;

    if (IsMagazineEmpty() && HasMagazines())
    {
        StopFire();
        OnMagazineEmpty.Broadcast(this);
    }
}

void ASTUBaseWeapon::ChangeMagazine()
{
    if (!CurrentAmmo.IsInfinite)
    {
        if (CurrentAmmo.NumMagazines == 0) return;
        CurrentAmmo.NumMagazines--;
    }
    CurrentAmmo.NumBullets = DefaultAmmo.NumBullets;
}

bool ASTUBaseWeapon::CanReload() const
{
    return CurrentAmmo.NumBullets < DefaultAmmo.NumBullets && HasMagazines();
}

const FWeaponUIData& ASTUBaseWeapon::GetUIData() const
{
    return UIData;
}

const FAmmoData& ASTUBaseWeapon::GetAmmoData() const
{
    return CurrentAmmo;
}

bool ASTUBaseWeapon::TryToAddAmmo(int32 MagazinesAmount)
{
    if (CurrentAmmo.IsInfinite || IsAmmoFull() || MagazinesAmount <= 0) return false;

    if (IsOutOfAmmo())
    {
        CurrentAmmo.NumMagazines = FMath::Clamp(MagazinesAmount, 0, DefaultAmmo.NumMagazines + 1);
        OnMagazineEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.NumMagazines < DefaultAmmo.NumMagazines)
    {
        const int32 NextMagazineAmount = CurrentAmmo.NumMagazines + MagazinesAmount;
        if (DefaultAmmo.NumMagazines - NextMagazineAmount >= 0)
        {
            CurrentAmmo.NumMagazines = NextMagazineAmount;
        }
        else
        {
            CurrentAmmo.NumMagazines = DefaultAmmo.NumMagazines;
            CurrentAmmo.NumBullets = DefaultAmmo.NumBullets;
        }
    }
    else
    {
        CurrentAmmo.NumBullets = DefaultAmmo.NumBullets;
    }

    return true;
}

bool ASTUBaseWeapon::IsOutOfAmmo() const
{
    return IsMagazineEmpty() && !HasMagazines();
}

bool ASTUBaseWeapon::IsMagazineEmpty() const
{
    return CurrentAmmo.NumBullets == 0;
}

bool ASTUBaseWeapon::HasMagazines() const
{
    return CurrentAmmo.NumMagazines > 0 || CurrentAmmo.IsInfinite;
}

bool ASTUBaseWeapon::IsAmmoFull() const
{
    return CurrentAmmo.NumMagazines == DefaultAmmo.NumMagazines && CurrentAmmo.NumBullets == DefaultAmmo.NumBullets;
}

void ASTUBaseWeapon::LogAmmo()
{
    FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.NumBullets) + "/" +
                       (CurrentAmmo.IsInfinite ? "Infinite" : FString::FromInt(CurrentAmmo.NumMagazines));
    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX,  //
        WeaponMesh,                                                //
        MuzzleSocketName,                                          //
        FVector::ZeroVector,                                       //
        FRotator::ZeroRotator,                                     //
        EAttachLocation::SnapToTarget,                             //
        true);
}
