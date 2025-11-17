// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GA_CombatBase.h"
#include "GA_Block.generated.h"

class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitInputRelease;
/**
 * C++ implementation for the Block combat ability (hold-to-block).
 * Applies the Blocking GE on activation and removes it on end.
 */
UCLASS()
class MONSTERSMASHER_API UGA_Block : public UGA_CombatBase
{
	GENERATED_BODY()
	
public:
	UGA_Block();

	// The Gameplay Effect to apply to enter the blocking state (must grant State.Blocking tag)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Block")
	TSubclassOf<class UGameplayEffect> BlockingStanceGE;

	// Handle to the applied GE so we can remove it when the ability ends
	FActiveGameplayEffectHandle BlockingEffectHandle;
	
	// Keep a reference so the task isn't GC'd unexpectedly
	UPROPERTY()
	UAbilityTask_WaitInputRelease* WaitInputReleaseTaskInstance;
	
	// Callback for when the input is released (from UAbilityTask_WaitInputRelease)
	UFUNCTION()
	void OnBlockInputReleased(float TimeWaited);

	// Called when the ability is activated.
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// Called when the ability ends (e.g., when the input is released).
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
};
