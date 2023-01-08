// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUWeaponComponent.h"
#include <Weapon/STUBaseWeapon.h>
#include <GameFramework/Character.h>
#include <Components/SceneComponent.h>
#include "Animations/STUEqiupFinishedAnimNotify.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

USTUWeaponComponent::USTUWeaponComponent()
{

    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
    Super::BeginPlay();

    InitAnimations();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    CurrentWeapon = nullptr;
    for (auto& Weapon : Weapons)
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

    for (auto& WeaponClass : WeaponClasses)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(WeaponClass);
        if (!Weapon) continue;

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
    ACharacter* Character = GetCharacter();
    if (!Character) return;

    if (CurrentWeapon)
    {
        CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

    CurrentWeapon = Weapons[WeaponIndex];
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
    if (!EquipAnimMontage) return;

    for (auto& NotifyEvent : EquipAnimMontage->Notifies)
    {
        auto EquipFinishedAnimNotify = Cast<USTUEqiupFinishedAnimNotify>(NotifyEvent.Notify);
        if (EquipFinishedAnimNotify)
        {
            EquipFinishedAnimNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
            break;
        }
    }
}

void USTUWeaponComponent::OnEquipFinished(class USkeletalMeshComponent* MeshComp)
{
    ACharacter* Character = GetCharacter();
    if (!Character || Character->GetMesh() != MeshComp) return;

    EqiupAnimInProgress = false;
    UE_LOG(LogWeaponComponent, Display, TEXT("Equip finished"));
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

bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EqiupAnimInProgress;
}

bool USTUWeaponComponent::CanEquip() const
{
    return !EqiupAnimInProgress;
}
