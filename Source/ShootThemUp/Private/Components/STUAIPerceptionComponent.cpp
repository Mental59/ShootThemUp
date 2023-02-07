// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUAIPerceptionComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUHealthComponent.h"
#include "Perception/AISense_Sight.h"

AActor* USTUAIPerceptionComponent::GetNearestEnemy() const
{
    const AAIController* Controller = Cast<AAIController>(GetOwner());
    if (!Controller) return nullptr;

    const APawn* Pawn = Controller->GetPawn();
    if (!Pawn) return nullptr;

    TArray<AActor*> PerceiveActors;
    GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PerceiveActors);

    float TheNearestActorDistance = MAX_FLT;
    AActor* TheNearestActor = nullptr;
    for (AActor* const Actor : PerceiveActors)
    {
        const USTUHealthComponent* HealthComponent = Actor->FindComponentByClass<USTUHealthComponent>();
        if (HealthComponent && !HealthComponent->IsDead())  // TODO: check if it's an enemy
        {
            const double CurrentDistance = (Actor->GetActorLocation() - Pawn->GetActorLocation()).Size();
            if (CurrentDistance < TheNearestActorDistance)
            {
                TheNearestActorDistance = CurrentDistance;
                TheNearestActor = Actor;
            }
        }
    }

    return TheNearestActor;
}
