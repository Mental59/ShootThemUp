// Shoot Them Up Game, All Rights Reserved.

#include "Components/STUHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"

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

    AActor* ComponentOwner = GetOwner();
    if (ComponentOwner)
    {
        ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
    }
}

void USTUHealthComponent::OnTakeAnyDamage(
    AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (Damage <= 0.0f || IsDead()) return;

    SetHealth(Health - Damage);

    ClearAutoHealTimer();

    if (IsDead())
    {
        OnDeath.Broadcast();
    }
    else if (AutoHeal)
    {
        SetAutoHealTimer();
    }

    PlayCameraShake();
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
