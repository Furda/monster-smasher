// Fill out your copyright notice in the Description page of Project Settings.


#include "ANS_HitBoxWindow.h"
#include "GameFramework/Actor.h"

void UANS_HitBoxWindow::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                    UAnimSequenceBase* Animation,
                                    float TotalDuration,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (AActor* Owner = MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("[HitBoxWindow Begin] %s - Tag: %s, Strength: %.2f"),
		       *Owner->GetName(),
		       *HitBoxTag.ToString(),
		       StrengthMultiplier);
	}
#endif
}

void UANS_HitBoxWindow::NotifyEnd(USkeletalMeshComponent* MeshComp,
                                  UAnimSequenceBase* Animation,
                                  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (AActor* Owner = MeshComp->GetOwner())
	{
		UE_LOG(LogTemp, Log, TEXT("[HitBoxWindow End] %s - Tag: %s"),
		       *Owner->GetName(),
		       *HitBoxTag.ToString());
	}
#endif
}
