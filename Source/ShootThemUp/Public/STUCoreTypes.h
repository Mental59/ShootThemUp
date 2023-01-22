// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "STUCoreTypes.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMagazineEmptySignature, class ASTUBaseWeapon*);

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, float);

USTRUCT(BlueprintType)
struct FAmmoData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (ClampMin = "1"))
    int32 NumBullets;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (EditCondition = "!IsInfinite", ClampMin = "0"))
    int32 NumMagazines;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    bool IsInfinite;
};

USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<class ASTUBaseWeapon> WeaponClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    class UAnimMontage* ReloadAnimMontage;
};

USTRUCT(BlueprintType)
struct FWeaponUIData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    class UTexture2D* MainIcon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    class UTexture2D* CrosshairIcon;
};

USTRUCT(BlueprintType)
struct FDecalData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    class UMaterialInterface* Material;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FVector Size = FVector(10.0f);

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float LifeTime = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    float FadeOutTime = 0.7f;
};

USTRUCT(BlueprintType)
struct FImpactData
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    class UNiagaraSystem* NiagaraEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FDecalData DecalData;
};