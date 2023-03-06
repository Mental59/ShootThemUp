// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUMenuWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* StartGameButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ExitGameButton;

    virtual void NativeOnInitialized() override;

private:
    UFUNCTION()
    void OnStartGame();

    UFUNCTION()
    void OnExitGame();
};
