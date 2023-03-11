// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "STUMenuWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public USTUBaseWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitGameButton;

    UPROPERTY(meta = (BindWidgetAnim), Transient)
    class UWidgetAnimation* HideAnimation;

    UPROPERTY(meta = (BindWidget))
    class UImage* ShowImage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    class USoundCue* StartGameSound;

    virtual void NativeOnInitialized() override;
    virtual void OnAnimationFinished_Implementation(const class UWidgetAnimation* Animation) override;

private:
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnExitGame();
};
