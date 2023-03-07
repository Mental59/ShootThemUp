// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STULevelContainerWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTULevelContainerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* LevelContainer;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> LevelItemWidgetClass;

    virtual void NativeOnInitialized() override;

private:
    UPROPERTY()
    TArray<class USTULevelItemWidget*> LevelItemWidgets;

    void InitLevelContainer();
    void OnLevelSelected(const FLevelData& Data);
    class USTUGameInstance* GetSTUGameInstance() const;
};
