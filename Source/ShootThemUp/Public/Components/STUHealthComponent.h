// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUHealthComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SHOOTTHEMUP_API USTUHealthComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USTUHealthComponent();

    FOnDeathSignature OnDeath;
    FOnHealthChangedSignature OnHealthChanged;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsDead() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent() const;

    UFUNCTION(BlueprintCallable, Category = "Health")
    bool IsHealthFull() const;

    float GetHealth() const;
    void SetHealth(float NewHealth);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "1.0"))
    float MaxHealth = 100.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal")
    bool AutoHeal = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.0", EditCondition = "AutoHeal"))
    float HealUpdateTime = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.0", EditCondition = "AutoHeal"))
    float HealDelay = 3.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Heal", meta = (ClampMin = "0.1", EditCondition = "AutoHeal"))
    float HealModifier = 5.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
    TSubclassOf<class UCameraShakeBase> CameraShake;

    virtual void BeginPlay() override;

private:
    float Health = 0.0f;
    FTimerHandle AutoHealTimerHandle;

    UFUNCTION()
    void OnTakeAnyDamage(
        AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    void OnAutoHealTimerFired();

    void SetAutoHealTimer();
    void ClearAutoHealTimer();

    void PlayCameraShake();
    void ReportDamageEvent(float Damage, class AController* InstigatedBy);
};
