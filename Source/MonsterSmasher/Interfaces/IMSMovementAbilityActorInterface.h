// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameFramework/CharacterMovementComponent.h" // Needed for UCharacterMovementComponent
#include "IMSMovementAbilityActorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType) // BlueprintType allows it to be implemented in BP
class UMSMovementAbilityActorInterface : public UInterface
{
	GENERATED_BODY()
};

/** * @brief Interface for Actors that support movement-related abilities.
  * 
  * IMSMovementAbilityActorInterface
  * 
  * This interface is designed to be implemented by characters that need to support movement-related abilities.
  * It allows Gameplay Abilities to interact with character-specific movement functions without tightly coupling
  * the abilities to the ACharacter class.
  */

class MONSTERSMASHER_API IMSMovementAbilityActorInterface
{
	GENERATED_BODY()

public:
	// Returns the CharacterMovementComponent of the character.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement Ability Interface")
	UCharacterMovementComponent* GetCharacterMovementComponent_MS() const;
	virtual UCharacterMovementComponent* GetCharacterMovementComponent_MS_Implementation() const = 0; // Pure virtual

	// Launches the character with a given velocity. -- Use Root Motion force instead of this if possible.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement Ability Interface")
	void LaunchCharacter_MS(FVector LaunchVelocity, bool bXYOverride, bool bZOverride);
	virtual void LaunchCharacter_MS_Implementation(FVector LaunchVelocity, bool bXYOverride, bool bZOverride) = 0;
	// Pure virtual

	/** Gets the last movement input vector. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement Ability Interface")
	FVector GetLastMovementInputVector_MS() const;
	virtual FVector GetLastMovementInputVector_MS_Implementation() const = 0; // Pure virtual

	/** Calls the default ACharacter Jump function. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement Ability Interface")
	void Jump_MS();
	virtual void Jump_MS_Implementation() = 0; // Pure virtual

	/**
	 * Attempts to play a montage on the character's skeletal mesh.
	 * @param Montage The montage to play.
	 * @param PlayRate The rate at which to play the montage.
	 * @param StartSection The starting section name.
	 * @return The duration of the montage that will be played, or 0.0 if failed.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement Ability Interface")
	float PlayMontage_MS(UAnimMontage* Montage, float PlayRate = 1.0f, FName StartSection = NAME_None);
	virtual float PlayMontage_MS_Implementation(UAnimMontage* Montage, float PlayRate = 1.0f,
	                                            FName StartSection = NAME_None) = 0;

	/** Stops any montage currently playing. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Movement Ability Interface")
	void StopMontage_MS(UAnimMontage* Montage);
	virtual void StopMontage_MS_Implementation(UAnimMontage* Montage) = 0;
};
