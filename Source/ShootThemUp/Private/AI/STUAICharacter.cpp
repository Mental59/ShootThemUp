// Shoot Them Up Game, All Rights Reserved.

#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/STUHealthComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAICharacter, All, All);

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
    AutoPossessAI = EAutoPossessAI::Disabled;
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

    HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
    HealthWidgetComponent->SetupAttachment(GetRootComponent());
    HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthWidgetComponent->SetDrawAtDesiredSize(true);
}

void ASTUAICharacter::OnTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    if (InstigatedBy && InstigatedBy->IsPlayerController() && InstigatedBy->GetPawn())
    {
        bool AreEnemies = STUUtils::AreEnemies(InstigatedBy, this->GetController());
        USTUHealthBarWidget* HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
        if(HealthBarWidget && AreEnemies)
        {
            const float DamageDistance = FVector::Distance(this->GetActorLocation(), InstigatedBy->GetPawn()->GetActorLocation());
            HealthBarWidget->UpdateHealthBarTimer(DamageDistance <= HealthBarMaxDrawDistance);
        }
    }
}

void ASTUAICharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthWidgetComponent);
    OnTakeAnyDamage.AddDynamic(this, &ASTUAICharacter::OnTakeDamage);
}

void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
    Super::OnHealthChanged(Health, HealthDelta);

    if(USTUHealthBarWidget* HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject()))
    {
        HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());
    }
}

void ASTUAICharacter::OnDeath()
{
    Super::OnDeath();

    const AAIController* STUController = Cast<AAIController>(Controller);
    if (STUController && STUController->BrainComponent)
    {
        STUController->BrainComponent->Cleanup();
    }
}
