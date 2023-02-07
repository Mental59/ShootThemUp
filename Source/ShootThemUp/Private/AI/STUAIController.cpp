// Shoot Them Up Game, All Rights Reserved.

#include "AI/STUAIController.h"
#include "AI/STUAICharacter.h"
#include "GameFramework/Pawn.h"

void ASTUAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (const ASTUAICharacter* STUCharacter = Cast<ASTUAICharacter>(InPawn))
    {
        RunBehaviorTree(STUCharacter->BehaviorTree);
    }
}
