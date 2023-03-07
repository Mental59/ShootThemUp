// Shoot Them Up Game, All Rights Reserved.


#include "Menu/UI/STULevelContainerWidget.h"
#include "STUGameInstance.h"
#include "Components/HorizontalBox.h"
#include "Menu/UI/STULevelItemWidget.h"

void USTULevelContainerWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();
    InitLevelContainer();
}

void USTULevelContainerWidget::InitLevelContainer()
{
    if(const USTUGameInstance* GameInstance = GetSTUGameInstance())
    {
        checkf(GameInstance->GetLevelsData().Num() != 0, TEXT("Levels data must not be empty"));
        
        if (LevelContainer)
        {
            LevelContainer->ClearChildren();
            for (const FLevelData& LevelData : GameInstance->GetLevelsData())
            {
                USTULevelItemWidget* LevelItemWidget = CreateWidget<USTULevelItemWidget>(GetWorld(), LevelItemWidgetClass);
                if (!LevelItemWidget) continue;
                
                LevelItemWidget->SetLevelData(LevelData);
                LevelItemWidget->OnLevelSelected.AddUObject(this, &USTULevelContainerWidget::OnLevelSelected);

                LevelContainer->AddChild(LevelItemWidget);
                LevelItemWidgets.Add(LevelItemWidget);
            }
        }

        if (GameInstance->GetStartUpLevel().LevelName.IsNone())
        {
            OnLevelSelected(GameInstance->GetLevelsData()[0]);
        } else
        {
            OnLevelSelected(GameInstance->GetStartUpLevel());
        }
    }
}

void USTULevelContainerWidget::OnLevelSelected(const FLevelData& Data)
{
    if(USTUGameInstance* GameInstance = GetSTUGameInstance())
    {
        GameInstance->SetStartUpLevel(Data);
        for (const auto& LevelItemWidget : LevelItemWidgets)
        {
            if (LevelItemWidget)
            {
                LevelItemWidget->SetSelected(Data.LevelId == LevelItemWidget->GetLevelData().LevelId);
            }
        }
    }
}

USTUGameInstance* USTULevelContainerWidget::GetSTUGameInstance() const
{
    if (!GetWorld()) return nullptr;
    return GetWorld()->GetGameInstance<USTUGameInstance>();
}
