// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "STUUtils.h"
#include "Perception/AISense_Damage.h"
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

USTUHealthComponent::USTUHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

float USTUHealthComponent::GetHealth() const
{
    return Health;
}

bool USTUHealthComponent::IsDead() const
{
    return FMath::IsNearlyZero(Health);
}

float USTUHealthComponent::GetHealthPercent() const
{
    return Health / MaxHealth;
}

bool USTUHealthComponent::IsHealthFull() const
{
    return FMath::IsNearlyEqual(Health, MaxHealth);
}

void USTUHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHealth(MaxHealth);

    if (AActor* ComponentOwner = GetOwner())
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
        ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
        ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    
}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
    UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
    ApplyDamage(Damage * GetPointDamageModifier(DamagedActor, BoneName), InstigatedBy);
}

void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
    FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
    ApplyDamage(Damage, InstigatedBy);
}

void USTUHealthComponent::OnAutoHealTimerFired()
{
    SetHealth(Health + HealModifier);

    if (IsHealthFull())
    {
        ClearAutoHealTimer();
    }
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
    const float NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
    const float HealthDelta = NextHealth - Health;
    Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

void USTUHealthComponent::SetAutoHealTimer()
{
    if (!GetWorld()) return;

    GetWorld()->GetTimerManager().SetTimer(
        AutoHealTimerHandle, this, &USTUHealthComponent::OnAutoHealTimerFired, HealUpdateTime, true, HealDelay);
}

void USTUHealthComponent::ClearAutoHealTimer()
{
    if (!GetWorld()) return;

    GetWorld()->GetTimerManager().ClearTimer(AutoHealTimerHandle);
}

void USTUHealthComponent::PlayCameraShake()
{
    if (IsDead()) return;

    const auto Player = Cast<APawn>(GetOwner());
    if (!Player) return;

    const auto Controller = Player->GetController<APlayerController>();
    if (!Controller || !Controller->PlayerCameraManager) return;

    Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}

void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
    if (GetWorld() && InstigatedBy && GetOwner() && InstigatedBy->GetPawn())
    {
        UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage,
            InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
    }
}

void USTUHealthComponent::KilledBy(AController* KillerController)
{
    if (!GetWorld()) return;
    if (ASTUGameModeBase* GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode()))
    {
        APawn* PlayerPawn = Cast<APawn>(GetOwner());
        AController* VictimController = PlayerPawn ? PlayerPawn->Controller : nullptr;
        GameMode->Killed(KillerController, VictimController);
    }
}

void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
    if (Damage <= 0.0f || IsDead()) return;

    if (const APawn* DamagedPawn = Cast<APawn>(GetOwner()))
    {
        if (!STUUtils::AreEnemies(DamagedPawn->GetController(), InstigatedBy)) return;
    }

    SetHealth(Health - Damage);

    ClearAutoHealTimer();

    if (IsDead())
    {
        KilledBy(InstigatedBy);
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        SetAutoHealTimer();
    }

    PlayCameraShake();
    ReportDamageEvent(Damage, InstigatedBy);
}

float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName) const
{
    if (const ACharacter* Character = Cast<ACharacter>(DamagedActor))
    {
        const UPhysicalMaterial* PhysicalMaterial = GetPhysicalMaterial(Character, BoneName);
        if (PhysicalMaterial && DamageModifiers.Contains(PhysicalMaterial))
        {
            return DamageModifiers[PhysicalMaterial];
        }
    }
    return 1.0f;
}

UPhysicalMaterial* USTUHealthComponent::GetPhysicalMaterial(const ACharacter* Character, const FName& BoneName) const
{
    if (const USkeletalMeshComponent* MeshComp = Character->GetMesh())
    {
        if (const FBodyInstance* BodyInstance = MeshComp->GetBodyInstance(BoneName))
        {
            return BodyInstance->GetSimplePhysicalMaterial();
        }
    }
    return nullptr;
}
