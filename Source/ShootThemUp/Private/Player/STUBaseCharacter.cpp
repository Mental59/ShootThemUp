// Shoot Them Up Game, All Rights Reserved.

#include "Player/STUBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
ASTUBaseCharacter::ASTUBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->bUsePawnControlRotation = false;

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;             // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);  // ...at this rotation rate
}

// Called when the game starts or when spawned
void ASTUBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    DefaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
}

// Called every frame
void ASTUBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASTUBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Move);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Look);

        // Running
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &ASTUBaseCharacter::Run);
        EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &ASTUBaseCharacter::StopRunning);
    }
}

void ASTUBaseCharacter::Move(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        IsMovingForward = MovementVector.Y > 0.0;

        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ASTUBaseCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
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
