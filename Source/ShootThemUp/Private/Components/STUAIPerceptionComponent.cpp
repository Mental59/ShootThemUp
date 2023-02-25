// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUAIPerceptionComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"

AActor* USTUAIPerceptionComponent::GetNearestEnemy() const
{
    const AAIController* Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const APawn* Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    TArray<AActor*> PerceivedActors;
    GetCurrentlyPerceivedActors(nullptr, PerceivedActors);

    float TheNearestActorDistance = MAX_FLT;
    AActor* TheNearestActor = nullptr;
    for (AActor* const PerceivedActor : PerceivedActors)
    {
        const USTUHealthComponent* HealthComponent = PerceivedActor->FindComponentByClass<USTUHealthComponent>();

        const APawn* PerceivedPawn = Cast<APawn>(PerceivedActor);
        bool AreEnemies = PerceivedPawn && STUUtils::AreEnemies(Controller, PerceivedPawn->GetController());

        if (HealthComponent && !HealthComponent->IsDead() && AreEnemies)
        {
            const double CurrentDistance = (PerceivedActor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < TheNearestActorDistance)
            {
                TheNearestActorDistance = CurrentDistance;
                TheNearestActor = PerceivedActor;
            }
        }
    }

    return TheNearestActor;
}
