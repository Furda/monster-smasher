// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MSGameplayAbility.generated.h"

// Define Activation Policy enum
// This is specific to the game design and how MSASC might interpret it,
UENUM(BlueprintType)
enum class EMSAbilityActivationPolicy : uint8
{
	// Tries to activate the ability when the input is triggered (e.g., a single press).
	OnInputTriggered UMETA(DisplayName = "On Input Triggered"),

	// Tries to activate the ability while the input is active (e.g., holding a button).
	// The ability will typically end when the input is released, or by other means.
	WhileInputActive UMETA(DisplayName = "While Input Active"),

	// The ability is activated automatically when it is granted to the Ability System Component.
	// Useful for passive abilities, or abilities that set up initial state.
	OnGranted UMETA(DisplayName = "On Granted"),
};

/**
 * Base class for all Gameplay Abilities in Monster Smasher.
 * Provides custom activation policies and other common functionality.
 */
UCLASS()
class MONSTERSMASHER_API UMSGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UMSGameplayAbility();

	// This dictates when and how the ability attempts to activate based on input.
	// NOTE: This does nothing right now
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability Activation")
	EMSAbilityActivationPolicy ActivationPolicy;

	// Getter for the activation policy
	EMSAbilityActivationPolicy GetActivationPolicy() const { return ActivationPolicy; }

	// Determine if this ability should be shown in the skill bar UI
	// This can be used by UI systems to filter and display abilities appropriately.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bShouldShowInSkillBar = false;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	// Called when the ability is cancelled for any reason (e.g., stun, new ability overriding it).
	// virtual void OnAbilityCancelled(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	//                                 const FGameplayAbilityActivationInfo ActivationInfo) override;

	// This is where we clean up any lingering effects or state changes.
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;
};
