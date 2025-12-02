// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "Input/MSInputConfig.h"
#include "MSPlayerController.generated.h"

class AMSPlayerState;
class UMSAbilitySystemComponent;
class UW_MSGameHUD;
class UInputMappingContext;

/**
 * The base Player Controller class of the project.
 * It manages the HUD and Inputs
 */
UCLASS()
class MONSTERSMASHER_API AMSPlayerController : public AModularPlayerController
{
	GENERATED_BODY()


	// =======================
	// Set up and overrides
	// =======================

public:
	UW_MSGameHUD* GetGameHUD() const;

protected:
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;


	// =======================
	// Input Mapping 
	// =======================

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;


	// =======================
	// HUD
	// =======================

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TSubclassOf<UW_MSGameHUD> GameHUDClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	bool bIsGameHUDInitialized = false;

	/** * Helper function to contain the common logic for initializing the HUD 
	 * with GAS components. Called by both BeginPlay (Server/Host) and OnRep_PlayerState (Client).
	 */
	void TryInitializeHUDWithGAS();

private:
	UPROPERTY()
	TObjectPtr<UW_MSGameHUD> GameHUD;

	// =======================
	// Player state and ASC properties
	// =======================

public:
	AMSPlayerState* GetMSPlayerState() const;
	UMSAbilitySystemComponent* GetMSAbilitySystemComponent() const;


	// =======================
	// Ability Binding
	// =======================

	// Function to handle binding ability inputs (will be called from SetupPlayerInputComponent)
	void BindAbilityInput(UEnhancedInputComponent* EnhancedInputComponent, UMSInputConfig* AbilityInputConfig);

protected:
	// UFUNCTION() // Needs to be marked as UFUNCTION() for the binding to work
	// void AbilityInputTagPressed(FGameplayTag InputTag);
	//
	// UFUNCTION()
	// void AbilityInputTagReleased(FGameplayTag InputTag);

	UFUNCTION()
	void AbilityInputIDPressed(EAbilityInputID InputID);

	UFUNCTION()
	void AbilityInputIDReleased(EAbilityInputID InputID);
};
