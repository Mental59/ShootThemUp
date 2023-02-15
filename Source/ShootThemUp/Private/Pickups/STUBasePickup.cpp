// Shoot Them Up Game, All Rights Reserved.

#include "Pickups/STUBasePickup.h"
#include <Components/SphereComponent.h>

DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All);

ASTUBasePickup::ASTUBasePickup()
{
    PrimaryActorTick.bCanEverTick = true;

    CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
    CollisionComponent->InitSphereRadius(50.0f);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    SetRootComponent(CollisionComponent);
}

void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor);
    if (GivePickupTo(Pawn))
    {
        PickupWasTaken();
    }
    else if (Pawn)
    {
        OverlappingPawns.Add(Pawn);
    }
}

void ASTUBasePickup::NotifyActorEndOverlap(AActor* OtherActor)
{
    Super::NotifyActorEndOverlap(OtherActor);

    const auto Pawn = Cast<APawn>(OtherActor);
    OverlappingPawns.Remove(Pawn);
}

void ASTUBasePickup::BeginPlay()
{
    Super::BeginPlay();

    check(CollisionComponent);

    GenerateRotationYaw();
}

bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
    return false;
}

void ASTUBasePickup::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));

    for (const auto OverlappingPawn : OverlappingPawns)
    {
        if (GivePickupTo(OverlappingPawn))
        {
            PickupWasTaken();
            break;
        }
    }
}

bool ASTUBasePickup::CouldBeTaken() const
{
    return !GetWorldTimerManager().IsTimerActive(RespawnTimerHandle);
}

void ASTUBasePickup::PickupWasTaken()
{
    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(false, true);
    }
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, RespawnTime);
}

void ASTUBasePickup::Respawn()
{
    GenerateRotationYaw();
    if (GetRootComponent())
    {
        GetRootComponent()->SetVisibility(true, true);
    }

    FTimerHandle CollisionTimerHandle;
    GetWorldTimerManager().SetTimer(CollisionTimerHandle, this, &ASTUBasePickup::EnableCollision, CollisionEnadleTime);
}

void ASTUBasePickup::GenerateRotationYaw()
{
    const float Direction = FMath::RandBool() ? 1.0f : -1.0f;
    RotationYaw = FMath::RandRange(1.0f, 2.0f) * Direction;
}

void ASTUBasePickup::EnableCollision()
{
    CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}
