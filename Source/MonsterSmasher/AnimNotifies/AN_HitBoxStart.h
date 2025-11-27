// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_HitBoxStart.generated.h"

/**
 * Simple notify to signal the start of a hitbox window.
 * Will trigger OnPlayMontageNotifyBegin when placed in a Montage.
 */
UCLASS()
class MONSTERSMASHER_API UAN_HitBoxStart : public UAnimNotify
{
	GENERATED_BODY()

public:
	// Designers set this in the montage 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Notify")
	FGameplayTag HitBoxTag;

	// Optional: send a damage multiplier, window ID, etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="HitBox")
	float StrengthMultiplier = 1.0f;

	// Executes when this notify is triggered in an animation 
	virtual void Notify(USkeletalMeshComponent* MeshComp,
	                    UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;
};
