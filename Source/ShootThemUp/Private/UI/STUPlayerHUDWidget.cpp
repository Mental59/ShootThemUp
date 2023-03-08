// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Player/STUPlayerState.h"
#include "STUGameModeBase.h"
#include "Components/ProgressBar.h"
#include "Player/STUPlayerState.h"
#include "Animation/WidgetAnimation.h"

void USTUPlayerHUDWidget::OnNewPawn(APawn* NewPawn)
{
    if (NewPawn)
    {
        USTUHealthComponent* HealthComponent = NewPawn->FindComponentByClass<USTUHealthComponent>();
        if (HealthComponent)
        {
            HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHUDWidget::OnHealthChanged);
        }
        UpdateHealthBar();
    }
}

void USTUPlayerHUDWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    if (APlayerController* PlayerController = GetOwningPlayer())
    {
        PlayerController->GetOnNewPawnNotifier().AddUObject(this, &USTUPlayerHUDWidget::OnNewPawn);
        OnNewPawn(GetOwningPlayerPawn());
    }
}

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    USTUHealthComponent* HealthComponent = GetComponent<USTUHealthComponent>();
    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}

bool USTUPlayerHUDWidget::GetWeaponUIData(FWeaponUIData& UIData) const
{
    USTUWeaponComponent* WeaponComponent = GetComponent<USTUWeaponComponent>();
    if (!WeaponComponent) return false;

    return WeaponComponent->GetWeaponUIData(UIData);
}

bool USTUPlayerHUDWidget::GetAmmoData(FAmmoData& AmmoData) const
{
    USTUWeaponComponent* WeaponComponent = GetComponent<USTUWeaponComponent>();
    if (!WeaponComponent) return false;

    return WeaponComponent->GetAmmoData(AmmoData);
}

bool USTUPlayerHUDWidget::IsPlayerAlive() const
{
    USTUHealthComponent* HealthComponent = GetComponent<USTUHealthComponent>();
    return HealthComponent && !HealthComponent->IsDead();
}

bool USTUPlayerHUDWidget::IsPlayerSpectating() const
{
    const APlayerController* Controller = GetOwningPlayer();
    return Controller && Controller->GetStateName() == NAME_Spectating;
}

void USTUPlayerHUDWidget::OnHealthChanged(float Health, float HealthDelta)
{
    if (HealthDelta < 0.0f)
    {
        OnTakeDamage();

        if (DamageAnimation && !IsAnimationPlaying(DamageAnimation))
        {
            PlayAnimation(DamageAnimation);
        }
    }
    UpdateHealthBar();
}

void USTUPlayerHUDWidget::UpdateHealthBar()
{
    if (HealthBar)
    {
        HealthBar->SetFillColorAndOpacity(GetHealthPercent() > PercentColorChangeThreshold ? DefaultColor : LowHealthColor);
    }
}

template <typename T>
T* USTUPlayerHUDWidget::GetComponent() const
{
    const APawn* Player = GetOwningPlayerPawn();
    if (!Player) return nullptr;
    return Cast<T>(Player->GetComponentByClass(T::StaticClass()));
}
