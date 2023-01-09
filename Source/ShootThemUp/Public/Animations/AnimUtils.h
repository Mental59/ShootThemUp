// Shoot Them Up Game, All Rights Reserved.

#pragma once

class AnimUtils
{
public:
    template <typename T>
    static T* FindFirstNotifyByClass(class UAnimSequenceBase* Animation)
    {
        if (!Animation) return nullptr;

        for (FAnimNotifyEvent& NotifyEvent : Animation->Notifies)
        {
            if (auto AnimNotify = Cast<T>(NotifyEvent.Notify))
            {
                return AnimNotify;
            }
        }

        return nullptr;
    }
};