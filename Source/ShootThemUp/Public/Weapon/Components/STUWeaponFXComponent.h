// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponFXComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUWeaponFXComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUWeaponFXComponent();

    void PlayImpactFX(const struct FHitResult& HitResult);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    FImpactData DefaultImpactData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TMap<class UPhysicalMaterial*, FImpactData> ImpactDataMap;

private:
    void SpawnNiagaraEffect(const FHitResult& HitResult, class UNiagaraSystem* NiagaraSystem) const;
    void SpawnDecal(const FHitResult& HitResult, const FDecalData& ImpactData) const;
    void PlayImpactSound(const FHitResult& HitResult, class USoundCue* ImpactSound) const;
};
