// Shoot Them Up Game, All Rights Reserved.

#include "AI/Services/STUChangeWeaponService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/STUWeaponComponent.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
    NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (const AAIController* Controller = OwnerComp.GetAIOwner())
    {
        if (USTUWeaponComponent* WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTUWeaponComponent>())
        {
            if (Probability > 0 && FMath::FRand() <= Probability || WeaponComponent->IsOutOfAmmo())
            {
                WeaponComponent->NextWeapon();
            }
        }
    }

    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
