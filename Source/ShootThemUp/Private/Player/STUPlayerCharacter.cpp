// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "Components/STUWeaponComponent.h"
#include "InputActionValue.h"

ASTUPlayerCharacter::ASTUPlayerCharacter(const FObjectInitializer& ObjInit) : Super(ObjInit)
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;
    SpringArmComponent->SocketOffset = FVector(0.0f, 100.0f, 80.0f);

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->bUsePawnControlRotation = false;
}

void ASTUPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ASTUPlayerCharacter::OnDeath()
{
    Super::OnDeath();
    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ASTUPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);
    check(WeaponComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Move);

        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Look);

        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASTUPlayerCharacter::Run);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASTUPlayerCharacter::StopRunning);

        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &ASTUPlayerCharacter::StartFire);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &ASTUPlayerCharacter::StopFire);

        EnhancedInputComponent->BindAction(NextWeaponAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::NextWeapon);

        EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, WeaponComponent, &USTUWeaponComponent::ReloadWeapon);
    }
}

void ASTUPlayerCharacter::Move(const FInputActionValue& Value)
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
}

void ASTUPlayerCharacter::Look(const FInputActionValue& Value)
{
    if (!Controller) return;

    FVector2D LookAxisVector = Value.Get<FVector2D>();

    AddControllerYawInput(LookAxisVector.X);
    AddControllerPitchInput(LookAxisVector.Y);
}

void ASTUPlayerCharacter::Run()
{
    WantsToRun = true;
}

void ASTUPlayerCharacter::StopRunning()
{
    WantsToRun = false;
}

void ASTUPlayerCharacter::StartFire()
{
    if (IsRunning()) return;
    WeaponComponent->StartFire();
}

void ASTUPlayerCharacter::StopFire()
{
    if (IsRunning()) return;
    WeaponComponent->StopFire();
}