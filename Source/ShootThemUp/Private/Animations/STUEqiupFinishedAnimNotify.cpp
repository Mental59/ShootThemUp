// Shoot Them Up Game, All Rights Reserved.

#include "Animations/STUEqiupFinishedAnimNotify.h"

void USTUEqiupFinishedAnimNotify::Notify(
    USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);
    OnNotified.Broadcast(MeshComp);
}
