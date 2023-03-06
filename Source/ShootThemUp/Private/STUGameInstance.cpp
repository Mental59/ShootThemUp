// Shoot Them Up Game, All Rights Reserved.

#include "STUGameInstance.h"

const FName& USTUGameInstance::GetStartUpLevelName() const
{
    return StartUpLevelName;
}

const FName& USTUGameInstance::GetMenuLevelName() const
{
    return MenuLevelName;
}
