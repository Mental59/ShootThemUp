// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUNeedAmmoDecorator.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUNeedAmmoDecorator : public UBTDecorator
{
    GENERATED_BODY()

public:
    USTUNeedAmmoDecorator();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TSubclassOf<class ASTUBaseWeapon> WeaponType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    bool NeedFullAmmo = true;

    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
