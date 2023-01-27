// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All);

ASTUBaseCharacter::ASTUBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
    HealthTextComponent->SetOwnerNoSee(true);

    WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 230.0f, 0.0f);
}

void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());
    check(GetCapsuleComponent());
    check(GetMesh());

    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
    OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
    HealthComponent->OnDeath.AddUObject(this, &ASTUBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ASTUBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ASTUBaseCharacter::OnGroundLanded);
}

void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Look);

        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Run);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASTUBaseCharacter::StopRunning);

        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ASTUBaseCharacter::StartFire);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ASTUBaseCharacter::StopFire);

        EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::NextWeapon);

        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::ReloadWeapon);
    }
}

void ASTUBaseCharacter::Move(const FInputActionValue& Value)
{
    if (!CanMove || !Controller) return;

    FVector2D MovementVector = Value.Get<FVector2D>();

    IsMovingForward = MovementVector.Y > 0.0 && FMath::IsNearlyZero(MovementVector.X);

    const FRotator Rotation = Controller->GetControlRotation();
    const FRotator YawRotation(0, Rotation.Yaw, 0);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    AddMovementInput(ForwardDirection, MovementVector.Y);
    AddMovementInput(RightDirection, MovementVector.X);

    if (!GetVelocity().IsZero())
    {
        WantsToTurnRight = WantsToTurnLeft = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
}

void ASTUBaseCharacter::Look(const FInputActionValue& Value)
{
    if (!Controller) return;

    FVector2D LookAxisVector = Value.Get<FVector2D>();

    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);

    TurnCharacter();
}

void ASTUBaseCharacter::TurnCharacter()
{
    if (!GetVelocity().IsZero()) return;

    const FRotator AimOffsets = GetAimOffsets();

    if (AimOffsets.Yaw > 90.0 && !WantsToTurnRight)
    {
        WantsToTurnRight = true;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }

    if (AimOffsets.Yaw < -90.0 && !WantsToTurnLeft)
    {
        WantsToTurnLeft = true;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }

    if (FMath::IsNearlyZero(AimOffsets.Yaw, 1.0E-4))
    {
        WantsToTurnRight = WantsToTurnLeft = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = false;
    }
}

void ASTUBaseCharacter::Run()
{
    if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
    {
        WantsToRun = true;
        MovementComponent->MaxWalkSpeed = IsMovingForward ? DefaultWalkSpeed * RunSpeedModifier : DefaultWalkSpeed;
    }
}

void ASTUBaseCharacter::StopRunning()
{
    if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
    {
        WantsToRun = false;
        MovementComponent->MaxWalkSpeed = DefaultWalkSpeed;
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
    const FVector Acceleration = GetCharacterMovement()->GetCurrentAcceleration();
    return !Acceleration.IsZero() && GroundSpeed > 3.0f;
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
    UE_LOG(LogBaseCharacter, Display, TEXT("Player %s is dead"), *GetName());

    // PlayAnimMontage(DeathAnimMontage);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    WeaponComponent->StopFire();

    GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    GetMesh()->SetSimulatePhysics(true);
}

void ASTUBaseCharacter::OnHealthChanged(float Health, float HealthDelta)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ASTUBaseCharacter::StartFire()
{
    if (IsRunning()) return;
    WeaponComponent->StartFire();
}

void ASTUBaseCharacter::StopFire()
{
    if (IsRunning()) return;
    WeaponComponent->StopFire();
}

void ASTUBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const double FallVelocityZ = -GetVelocity().Z;
    UE_LOG(LogBaseCharacter, Display, TEXT("On landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X) return;

    const double FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
    TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);

    UE_LOG(LogBaseCharacter, Display, TEXT("Final Damage: %f"), FinalDamage);
}
