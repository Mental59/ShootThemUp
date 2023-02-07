#pragma once

class STUUtils
{
public:
    template <typename T>
    static T* GetSTUPlayerComponent(class AActor* Actor)
    {
        if (!Actor) return nullptr;
        return Cast<T>(Actor->GetComponentByClass(T::StaticClass()));
    }
};