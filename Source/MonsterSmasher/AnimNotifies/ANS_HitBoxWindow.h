// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GameplayTagContainer.h"
#include "ANS_HitBoxWindow.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "HitBox Window"))
class MONSTERSMASHER_API UANS_HitBoxWindow : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	//A gameplay tag identifying the type of window
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitBox")
	FGameplayTag HitBoxTag;

	// Optional per-window damage multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitBox")
	float StrengthMultiplier = 1.f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
	                         UAnimSequenceBase* Animation,
	                         float TotalDuration,
	                         const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp,
	                       UAnimSequenceBase* Animation,
	                       const FAnimNotifyEventReference& EventReference) override;
};
