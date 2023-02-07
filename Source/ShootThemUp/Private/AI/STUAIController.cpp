// Shoot Them Up Game, All Rights Reserved.

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "Components/STUAIPerceptionComponent.h"

ASTUAIController::ASTUAIController()
{
    STUAIPerceptionComponent = CreateDefaultSubobject<USTUAIPerceptionComponent>("PerceptionComponent");
    SetPerceptionComponent(*STUAIPerceptionComponent);
}

void ASTUAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    AActor* AimAtActor = STUAIPerceptionComponent->GetNearestEnemy();
    SetFocus(AimAtActor);
}

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (const ASTUAICharacter* STUCharacter = Cast<ASTUAICharacter>(InPawn))
    {
        RunBehaviorTree(STUCharacter->BehaviorTree);
    }
}
