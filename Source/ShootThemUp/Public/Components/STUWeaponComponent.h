// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponComponent();

    virtual void StartFire();
    virtual void StopFire();
    virtual void NextWeapon();
    void ReloadWeapon();

    bool GetWeaponUIData(FWeaponUIData& UIData) const;
    bool GetAmmoData(FAmmoData& AmmoData) const;

    bool TryToAddAmmo(TSubclassOf<class ASTUBaseWeapon> WeaponType, int32 MagazinesAmount);
    bool NeedAmmo(TSubclassOf<class ASTUBaseWeapon> WeaponType, bool NeedFullAmmo) const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TArray<FWeaponData> WeaponData;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponEquipSocketName = "WeaponSocket";

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    FName WeaponArmorySocketName = "ArmorySocket";

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* EquipAnimMontage;

    UPROPERTY()
    class ASTUBaseWeapon* CurrentWeapon = nullptr;

    UPROPERTY()
    TArray<class ASTUBaseWeapon*> Weapons;

    int32 CurrentWeaponIndex = 0;

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool CanFire() const;
    bool CanEquip() const;

    void EquipWeapon(int32 WeaponIndex);

    class ACharacter* GetCharacter() const;

private:
    UPROPERTY()
    class UAnimMontage* CurrentReloadAnimMontage = nullptr;

    bool EqiupAnimInProgress = false;
    bool ReloadAnimInProgress = false;

    void SpawnWeapons();
    void AttachWeaponToSocket(class ASTUBaseWeapon* Weapon, class USceneComponent* SceneComponent, const FName& SocketName);

    void PlayAnimMontage(class UAnimMontage* AnimMontage);
    void InitAnimations();
    void OnEquipFinished(class USkeletalMeshComponent* MeshComp);
    void OnReloadFinished(class USkeletalMeshComponent* MeshComp);

    bool CanReload() const;

    void OnMagazineEmpty(class ASTUBaseWeapon* EmptyWeapon);
    void ChangeMagazine();
};
