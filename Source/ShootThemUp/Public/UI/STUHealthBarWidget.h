// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUHealthBarWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    void SetHealthPercent(float HealthPercent);
    void UpdateHealthBarTimer(bool Visible);

protected:
    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthProgressBar;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0"))
    float HideBarDelay = 2.0f;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    float PercentColorChangeThreshold = 0.4f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor DefaultColor = FLinearColor::White;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    FLinearColor LowHealthColor = FLinearColor::Red;

    virtual void NativeOnInitialized() override;

private:
    FTimerHandle HealthBarTimerHandle;
    
    void HideHealthBar();
};
