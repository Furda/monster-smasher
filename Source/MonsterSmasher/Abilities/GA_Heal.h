// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "GA_Heal.generated.h"

class AMSCharacterBase;

/**
 * 
 */
UCLASS()
class MONSTERSMASHER_API UGA_Heal : public UMSGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Heal();

protected:
	// virtual UGameplayEffect* GetCooldownGameplayEffect() const override;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	void ApplyHealingEffect(const AMSCharacterBase* TargetCharacter);

	UPROPERTY(EditDefaultsOnly, Category = "Heal")
	TSubclassOf<UGameplayEffect> HealingEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Heal")
	float HealingAmount = 20.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Heal")
	FGameplayTag HealMagnitudeTag;
};
