// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/STUCharacterMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

ASTUBaseCharacter::ASTUBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 150.0f, 0.0f);
}

void ASTUBaseCharacter::SetPlayerColor(const FLinearColor& PlayerColor)
{

    if (UMaterialInstanceDynamic* MaterialInstance = GetMesh()->CreateAndSetMaterialInstanceDynamic(0))
    {
        MaterialInstance->SetVectorParameterValue(MaterialColorName, PlayerColor);
    }
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(GetCharacterMovement());
    check(GetCapsuleComponent());
    check(GetMesh());

    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TurnCharacter();
}

void ASTUBaseCharacter::TurnCharacter()
{
    if (!GetVelocity().IsZero())
    {
        WantsToTurnRight = WantsToTurnLeft = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        return;
    }

    const FRotator AimOffsets = GetAimOffsets();

    if (AimOffsets.Yaw > 90.0 && !WantsToTurnRight)
    {
        WantsToTurnRight = true;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
    else
    {
        WantsToTurnRight = false;
    }

    if (AimOffsets.Yaw < -90.0 && !WantsToTurnLeft)
    {
        WantsToTurnLeft = true;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
    else
    {
        WantsToTurnLeft = false;
    }

    if (FMath::IsNearlyZero(AimOffsets.Yaw, 1.0E-4))
    {
        WantsToTurnRight = WantsToTurnLeft = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = false;
    }
}

bool ASTUBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}

bool ASTUBaseCharacter::ShouldMove() const
{
    const FVector Velocity = GetCharacterMovement()->Velocity;
    const float GroundSpeed = FMath::Sqrt(Velocity.X * Velocity.X + Velocity.Y * Velocity.Y);
    return GroundSpeed > 3.0f;
}

FRotator ASTUBaseCharacter::GetAimOffsets() const
{
    const FVector AimDirWS = GetBaseAimRotation().Vector();
    const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
    const FRotator AimRotLS = AimDirLS.Rotation();

    return AimRotLS;
}

void ASTUBaseCharacter::OnDeath()
{
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta) {}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const double FallVelocityZ = -GetVelocity().Z;

    if (FallVelocityZ < LandedDamageVelocity.X) return;

    const double FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}
