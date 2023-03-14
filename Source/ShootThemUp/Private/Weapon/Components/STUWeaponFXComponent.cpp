// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult)
{
    FImpactData ImpactData = DefaultImpactData;

    if (HitResult.PhysMaterial.IsValid())
    {
        const UPhysicalMaterial* PhysMaterial = HitResult.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMaterial))
        {
            ImpactData = ImpactDataMap[PhysMaterial];
        }
    }

    SpawnNiagaraEffect(HitResult, ImpactData.NiagaraEffect);
    SpawnDecal(HitResult, ImpactData.DecalData);
    PlayImpactSound(HitResult, ImpactData.ImpactSound);
}

void USTUWeaponFXComponent::SpawnNiagaraEffect(const FHitResult& HitResult, UNiagaraSystem* NiagaraSystem) const
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
}

void USTUWeaponFXComponent::SpawnDecal(const FHitResult& HitResult, const FDecalData& DecalData) const
{
    UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
        DecalData.Material,
        DecalData.Size,  
        HitResult.ImpactPoint,
        HitResult.ImpactNormal.Rotation());
    
    if (DecalComponent)
    {
        DecalComponent->SetFadeScreenSize(0.0f);
        DecalComponent->SetFadeOut(DecalData.LifeTime, DecalData.FadeOutTime);
    }
}

void USTUWeaponFXComponent::PlayImpactSound(const FHitResult& HitResult, USoundCue* ImpactSound) const
{
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.ImpactPoint);
}
