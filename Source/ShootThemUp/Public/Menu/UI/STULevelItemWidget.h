// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUCoreTypes.h"
#include "STULevelItemWidget.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTULevelItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    FOnLevelSelectedSignature OnLevelSelected;

    void SetLevelData(const FLevelData& Data);
    const FLevelData& GetLevelData() const;

    void SetSelected(bool IsSelected);

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* SelectLevelButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* LevelNameTextBlock;

    UPROPERTY(meta = (BindWidget))
    class UImage* LevelImage;

    UPROPERTY(meta = (BindWidget))
    class UImage* FrameImage;

    virtual void NativeOnInitialized() override;

private:
    FLevelData LevelData;

    UFUNCTION()
    void OnLevelClicked();
};
