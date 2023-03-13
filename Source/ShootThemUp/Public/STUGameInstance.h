// Shoot Them Up Game, All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "STUCoreTypes.h"
#include "STUGameInstance.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    
    const FLevelData& GetStartUpLevel() const;
    void SetStartUpLevel(const FLevelData& LevelData);

    const TArray<FLevelData>& GetLevelsData() const;
    
    const FName& GetMenuLevelName() const;

    void ToggleVolume() const;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    TArray<FLevelData> LevelsData;
    
    UPROPERTY(EditDefaultsOnly, Category = "Game")
    FName MenuLevelName = NAME_None;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    class USoundClass* MasterSoundClass;

private:
    FLevelData StartUpLevel; 
};
