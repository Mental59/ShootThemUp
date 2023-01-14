// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"

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

template <typename T>
T* USTUPlayerHUDWidget::GetComponent() const
{
    const APawn* Player = GetOwningPlayerPawn();
    if (!Player) return nullptr;
    return Cast<T>(Player->GetComponentByClass(T::StaticClass()));
}
