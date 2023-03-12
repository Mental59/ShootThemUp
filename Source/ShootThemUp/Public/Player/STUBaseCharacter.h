// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ASTUBaseCharacter(const FObjectInitializer& ObjInit);

    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool ShouldMove() const;

    UFUNCTION(BlueprintCallable, Category = "Aim")
    FRotator GetAimOffsets() const;

    void SetPlayerColor(const FLinearColor& PlayerColor);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    class USTUHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    class USTUWeaponComponent* WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    class UAnimMontage* DeathAnimMontage;

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(900.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    float LifeSpanOnDeath = 5.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Material")
    FName MaterialColorName = "Paint Color";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0", ClampMax = "180"))
    float TurnInPlaceAngle = 90.0f;
    
    UPROPERTY(BlueprintReadWrite, Category = "Movement")
    bool CanMove = true;

    UPROPERTY(BlueprintReadWrite, Category = "Movement")
    bool WantsToTurnRight = false;

    UPROPERTY(BlueprintReadWrite, Category = "Movement")
    bool WantsToTurnLeft = false;

    UPROPERTY(BlueprintReadWrite, Category = "Movement")
    bool WantsToRun = false;

    UPROPERTY(BlueprintReadWrite, Category = "Movement")
    bool IsMovingForward = false;

    virtual void BeginPlay() override;

    virtual void OnHealthChanged(float Health, float HealthDelta);

    virtual void OnDeath();

private:
    bool IsTurning = false;
    
    UFUNCTION()
    void OnGroundLanded(const FHitResult& Hit);

    void TurnInPlace();
};
