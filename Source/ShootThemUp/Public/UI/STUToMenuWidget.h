// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUToMenuWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUToMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* ToMenuButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnToMenu();
};
