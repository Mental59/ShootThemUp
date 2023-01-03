// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBaseWeapon.generated.h"

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ASTUBaseWeapon();

    virtual void Fire();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    class USkeletalMeshComponent* WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Shooting")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting", meta = (ClampMin = "0.0"))
    float TraceMaxDistance = 1500.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Shooting", meta = (ClampMin = "0.0", ClampMax = "180.0"))
    float MaxAngleBetweenPlayerAndTrace = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (ClampMin = "0.0"))
    float DamageAmount = 10.0f;

    virtual void BeginPlay() override;

    void MakeShot();

    void LineTrace(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd);

    bool GetCameraTraceData(FVector& TraceStart, FVector& TraceEnd) const;

    float GetAngleBetweenPlayerAndTrace(const FVector& ImpactPoint, const ACharacter* Player) const;

    void DrawDebugGeometry(const FHitResult& HitResult) const;

    void MakeDamageToActor(const FHitResult& HitResult);

    APlayerController* GetPlayerController() const;

    ACharacter* GetPlayer() const;

    FVector GetMuzzleWorldLocation() const;
};
