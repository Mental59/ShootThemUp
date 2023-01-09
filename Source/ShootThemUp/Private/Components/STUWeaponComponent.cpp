// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUWeaponComponent.h"
#include <Weapon/STUBaseWeapon.h>
#include <GameFramework/Character.h>
#include <Components/SceneComponent.h>
#include "Animations/STUEqiupFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "Animations/AnimUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

constexpr static int32 MAX_WEAPON_NUM = 2;

USTUWeaponComponent::USTUWeaponComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    checkf(WeaponData.Num() == MAX_WEAPON_NUM, TEXT("Only %i weapons must be defined"), MAX_WEAPON_NUM);

    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (ASTUBaseWeapon* Weapon : Weapons)
    {
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
    }
    Weapons.Empty();

    Super::EndPlay(EndPlayReason);
}

ACharacter* USTUWeaponComponent::GetCharacter() const
{
    return Cast<ACharacter>(GetOwner());
}

void USTUWeaponComponent::SpawnWeapons()
{
    if (!GetWorld()) return;

    ACharacter* Character = GetCharacter();
    if (!Character) return;

    for (FWeaponData& OneWeaponData : WeaponData)
    {
        ASTUBaseWeapon* Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;

        Weapon->OnMagazineEmpty.AddUObject(this, &USTUWeaponComponent::OnMagazineEmpty);
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
}

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent) return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);
}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if (WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
    }

    ACharacter* Character = GetCharacter();
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
    const FWeaponData* CurrentWeaponData =
        WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass(); });
    CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;

    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    EqiupAnimInProgress = true;
    PlayAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontage)
{
    ACharacter* Character = GetCharacter();
    if (!Character) return;

    Character->PlayAnimMontage(AnimMontage);
}

void USTUWeaponComponent::InitAnimations()
{
    if (auto EquipFinishedNotify = AnimUtils::FindFirstNotifyByClass<USTUEqiupFinishedAnimNotify>(EquipAnimMontage))
    {
        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
    {
        UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is not set"));
        checkNoEntry();
    }

    for (FWeaponData& OneWeaponData : WeaponData)
    {
        if (auto ReloadFinishedNotify = AnimUtils::FindFirstNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage))
        {
            ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
        }
        else
        {
            UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is not set"));
            checkNoEntry();
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(class USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = GetCharacter();
    if (!Character || Character->GetMesh() != MeshComp) return;

    EqiupAnimInProgress = false;
}

void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = GetCharacter();
    if (!Character || Character->GetMesh() != MeshComp) return;

    ReloadAnimInProgress = false;
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire()) return;
    CurrentWeapon->StartFire();
}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    if (!CanEquip()) return;
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::ReloadWeapon()
{
    ChangeMagazine();
}

void USTUWeaponComponent::OnMagazineEmpty()
{
    ChangeMagazine();
}

void USTUWeaponComponent::ChangeMagazine()
{
    if (!CanReload()) return;
    CurrentWeapon->StopFire();
    CurrentWeapon->ChangeMagazine();
    ReloadAnimInProgress = true;
    PlayAnimMontage(CurrentReloadAnimMontage);
}

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EqiupAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EqiupAnimInProgress && !ReloadAnimInProgress;
}

bool USTUWeaponComponent::CanReload() const
{
    return CurrentWeapon && !EqiupAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}
