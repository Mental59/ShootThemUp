// Shoot Them Up Game, All Rights Reserved.

#include "STUGameInstance.h"

void USTUGameInstance::Init()
{
    Super::Init();
    for (int32 i = 0; i < LevelsData.Num(); i++)
    {
        LevelsData[i].LevelId = i;
    }
}

const FLevelData& USTUGameInstance::GetStartUpLevel() const
{
    return StartUpLevel;
}

void USTUGameInstance::SetStartUpLevel(const FLevelData& LevelData)
{
    StartUpLevel = LevelData;
}

const TArray<FLevelData>& USTUGameInstance::GetLevelsData() const
{
    return LevelsData;
}

const FName& USTUGameInstance::GetMenuLevelName() const
{
    return MenuLevelName;
}
