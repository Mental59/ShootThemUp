// Shoot Them Up Game, All Rights Reserved.

#include "AI/STUNextLocationTaskNode.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"

USTUNextLocationTaskNode::USTUNextLocationTaskNode()
{
    NodeName = "Next Location";
}

EBTNodeResult::Type USTUNextLocationTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const AAIController* Controller = OwnerComp.GetAIOwner();
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Controller || !Blackboard) return EBTNodeResult::Failed;

    APawn* Pawn = Controller->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(Pawn);
    if (!NavigationSystem) return EBTNodeResult::Failed;

    FNavLocation ResultLocation;
    FVector Location = Pawn->GetActorLocation();
    if (SelfCenter)
    {
        AActor* CenterActor = Cast<AActor>(Blackboard->GetValueAsObject(CenterActorKey.SelectedKeyName));
        if (!CenterActor) return EBTNodeResult::Failed;
        Location = CenterActor->GetActorLocation();
    }

    bool IsLocationFound = NavigationSystem->GetRandomReachablePointInRadius(Location, Radius, ResultLocation);
    if (!IsLocationFound) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, ResultLocation.Location);
    return EBTNodeResult::Succeeded;
}
