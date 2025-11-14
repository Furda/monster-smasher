// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_CombatBase.h"
#include "GA_LightAttack.generated.h"

/**
 * C++ implementation for the Light Attack combat ability.
 * Handles playing the montage and automatically ending the ability.
 */
UCLASS()
class MONSTERSMASHER_API UGA_LightAttack : public UGA_CombatBase
{
	GENERATED_BODY()
	
public:
	UGA_LightAttack();

	// Montage to play for the attack animation. Set this in the Blueprint child.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Light Attack")
	UAnimMontage* AttackMontage;

	// Called when the ability is activated.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	// Called when the attack montage finishes, is interrupted, or cancelled.
	UFUNCTION()
	void OnMontageFinished();
};
