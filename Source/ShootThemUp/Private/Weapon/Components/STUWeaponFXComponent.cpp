// Shoot Them Up Game, All Rights Reserved.

#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

USTUWeaponFXComponent::USTUWeaponFXComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& HitResult)
{
    auto ImpactData = DefaultImpactData;

    if (HitResult.PhysMaterial.IsValid())
    {
        const auto PhysMaterial = HitResult.PhysMaterial.Get();
        if (ImpactDataMap.Contains(PhysMaterial))
        {
            ImpactData = ImpactDataMap[PhysMaterial];
        }
    }

    SpawnNiagaraEffect(HitResult, ImpactData.NiagaraEffect);
    SpawnDecal(HitResult, ImpactData);
}

void USTUWeaponFXComponent::SpawnNiagaraEffect(const FHitResult& HitResult, UNiagaraSystem* NiagaraSystem)
{
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, HitResult.ImpactPoint, HitResult.ImpactNormal.Rotation());
}

void USTUWeaponFXComponent::SpawnDecal(const FHitResult& HitResult, const FImpactData& ImpactData)
{
    UDecalComponent* DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),  //
        ImpactData.DecalData.Material,                                                    //
        ImpactData.DecalData.Size,                                                        //
        HitResult.ImpactPoint,                                                            //
        HitResult.ImpactNormal.Rotation());

    if (DecalComponent)
    {
        DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
    }
}
