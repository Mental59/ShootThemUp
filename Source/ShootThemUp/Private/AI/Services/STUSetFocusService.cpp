// Shoot Them Up Game, All Rights Reserved.

#include "AI/Services/STUSetFocusService.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"

void USTUSetFocusService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    AAIController* Controller = OwnerComp.GetAIOwner();
    if (!Controller || !Blackboard) return;

    if (SetFocus)
    {
        AActor* FocusActor = Cast<AActor>(Blackboard->GetValueAsObject(FocusActorKey.SelectedKeyName));
        if (FocusActor) Controller->SetFocus(FocusActor);
    }
    else
    {
        Controller->SetFocus(nullptr);
    }
}
