// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "GA_CombatBase.generated.h"

/**
 * Base class for all abilities related to combat (attacks, blocks, dodges).
 * Used to implement shared checks, such as preventing action while stunned.
 */
UCLASS()
class MONSTERSMASHER_API UGA_CombatBase : public UMSGameplayAbility
{
	GENERATED_BODY()
	
public:
	// This tag is expected to be granted by a Gameplay Effect when the character is blocking.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FGameplayTag BlockingTag;
	
	// This tag is expected to be granted by a Gameplay Effect when the character is stunned.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	FGameplayTag StunnedTag;

	// Override CanActivateAbility to check if the character is in a state that prevents combat actions.
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

};
