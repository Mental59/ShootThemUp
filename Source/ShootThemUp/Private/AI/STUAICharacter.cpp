// Shoot Them Up Game, All Rights Reserved.

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogAICharacter, All, All);

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = ASTUAIController::StaticClass();

    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    bUseControllerRotationPitch = false;
    if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
    {
        MovementComp->bUseControllerDesiredRotation = true;
        MovementComp->bOrientRotationToMovement = false;
        MovementComp->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
    }
}
