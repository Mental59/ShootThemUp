// Shoot Them Up Game, All Rights Reserved.

#include "UI/STUPlayerHUDWidget.h"
#include "Components/STUHealthComponent.h"

float USTUPlayerHUDWidget::GetHealthPercent() const
{
    const APawn* Player = GetOwningPlayerPawn();
    if (!Player) return 0.0f;

    const UActorComponent* Component = Player->GetComponentByClass(USTUHealthComponent::StaticClass());
    const USTUHealthComponent* HealthComponent = Cast<USTUHealthComponent>(Component);

    if (!HealthComponent) return 0.0f;

    return HealthComponent->GetHealthPercent();
}
