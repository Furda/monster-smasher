// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_CombatBase.h"
#include "GA_LightAttack.generated.h"

/**
 * C++ implementation for the Light Attack combat ability.
 * Handles playing the montage and uses an Anim Notify to determine the hit-box window.
 */
UCLASS()
class MONSTERSMASHER_API UGA_LightAttack : public UGA_CombatBase
{
	GENERATED_BODY()

public:
	UGA_LightAttack();

	// Montage to play for the attack animation. Must contain the "HitBoxStart" Anim Notify.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Light Attack")
	UAnimMontage* AttackMontage;

	// Begin UGameplayAbility interface
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
	// End UGameplayAbility interface

protected:
	// Called when the attack montage finishes, is interrupted, or cancelled.
	UFUNCTION()
	void OnMontageFinished();

	// Called by the custom task (via Anim Notify) when the hit-box should be active.
	UFUNCTION()
	void OnHitBoxOpen();

	// Called if the HitBox task is canceled before the notify fires (e.g., ability cancelled mid-swing).
	UFUNCTION()
	void OnHitBoxTaskCancelled();
};
