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
    bool IsLocationFound = NavigationSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, ResultLocation);
    if (!IsLocationFound) return EBTNodeResult::Failed;

    Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, ResultLocation.Location);
    return EBTNodeResult::Succeeded;
}
