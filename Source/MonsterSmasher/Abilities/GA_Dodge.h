// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "GA_Dodge.generated.h"

/**
 * UGA_Dodge is a gameplay ability class that enables a dash or dodge mechanic.
 * It extends from UMSGameplayAbility to utilize Unreal Engine's Gameplay Ability System.
 * The ability applies a directional dash force to the actor when activated.
 */
UCLASS()
class MONSTERSMASHER_API UGA_Dodge : public UMSGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Dodge();

protected:
	// Helper function to calculate the cost of the ability
	float GetStaminaCost() const;

	// virtual UGameplayEffect* GetCooldownGameplayEffect() const override;

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                       OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                       const FGameplayAbilityActivationInfo ActivationInfo) const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	// Callback for when the dash duration ends (from UAbilityTask_WaitDelay)
	UFUNCTION()
	void OnDodgeFinished();

	UFUNCTION()
	void OnDodgeMontageCompleted();

	UPROPERTY(EditDefaultsOnly, Category = "Dodge | Animation")
	TObjectPtr<UAnimMontage> DodgeMontage;

	// Define dash force and duration
	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeForce = 2000.0f; // Adjust as needed

	UPROPERTY(EditDefaultsOnly, Category = "Dodge")
	float DodgeDuration = 0.2f; // Adjust as needed

	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	float StaminaCost = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Costs")
	FGameplayTag CostMagnitudeTag;
};
