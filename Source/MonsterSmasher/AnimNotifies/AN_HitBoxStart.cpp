// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_HitBoxStart.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"

void UAN_HitBoxStart::Notify(USkeletalMeshComponent* MeshComp,
                             UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr)
	{
		{
			UE_LOG(LogTemp, Log, TEXT("[HitBoxStart] %s - Tag: %s, Strength: %.2f"),
			       *Owner->GetName(),
			       *HitBoxTag.ToString(),
			       StrengthMultiplier);
		}
	}
#endif
}
