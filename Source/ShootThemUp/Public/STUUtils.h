#pragma once

#include "Player/STUPlayerState.h"
#include "Internationalization/Text.h"

class STUUtils
{
public:
    template <typename T>
    static T* GetSTUPlayerComponent(class AActor* Actor)
    {
        if (!Actor) return nullptr;
        return Cast<T>(Actor->GetComponentByClass(T::StaticClass()));
    }

    bool static AreEnemies(const AController* Controller1, const AController* Controller2)
    {
        if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

        const ASTUPlayerState* PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
        const ASTUPlayerState* PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

        return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
    }

    static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }

    static FText TextFromFloat(float Number) { return FText::FromString(FString::SanitizeFloat(Number)); }
};