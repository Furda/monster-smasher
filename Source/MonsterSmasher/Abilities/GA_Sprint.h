// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "GA_Sprint.generated.h"

class UCharacterMovementComponent;

/**
 * @class UGA_Sprint
 * @brief A gameplay ability class responsible for handling the sprint mechanic of the character.
 *
 * This ability enhances the character's movement speed for the duration of the sprint,
 * responding to player's input to initiate and end the ability as needed.
 * It includes activation and termination functionalities, along with a callback for input release.
 */
UCLASS()
class MONSTERSMASHER_API UGA_Sprint : public UMSGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_Sprint();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

	// Callback for when the input is released (from UAbilityTask_WaitInputRelease)
	UFUNCTION()
	void OnSprintInputReleased(float TimeWaited);

	// Store a reference to the CharacterMovementComponent for easy access
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;

	// Store the original walk speed to restore it when sprinting ends
	float OriginalWalkSpeed;

	// Define the sprint speed
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
	float SprintSpeedRate = 1.5f; // Adjust as needed
};
