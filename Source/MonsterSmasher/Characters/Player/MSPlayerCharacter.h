// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Base/MSCharacterBase.h"
#include "MSPlayerCharacter.generated.h"


class UWeaponManagerComponent;
class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class AMSPlayerState;
class UInputAction;
class UMSInputConfig;
class UInputMappingContext;

/**
 * AMSPlayerCharacter: Player-controlled character, gets its GAS components from PlayerState.
 */
UCLASS()
class MONSTERSMASHER_API AMSPlayerCharacter : public AMSCharacterBase
{
	GENERATED_BODY()

	// =======================
	// Set up and overrides
	// =======================
	
public:
	AMSPlayerCharacter();

	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	// Called when the PlayerController possesses this character (Server-side)
	virtual void PossessedBy(AController* NewController) override;

	// Called when the PlayerState is replicated to the client (Client-side)
	virtual void OnRep_PlayerState() override;

	
	// =======================
	// Weapon Manager Component 
	// =======================
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWeaponManagerComponent> WeaponManager;
	
	// =======================
	// Basic movement set up
	// =======================
	
protected:
	// Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Input functions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	
	// =======================
	// GAS Set up and overrides
	// =======================
public:
	// This will return the ASC from the PlayerState.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Return AttributeSet from the PlayerState
	virtual UMSAttributeSet* GetAttributeSet() const override;

protected:
	virtual void InitAbilitySystemAndAttributes() override;
	virtual void GrantStartingAbilities() override;
	
	
	// =======================
	// Ability Binding
	// =======================
	
protected:
	// Input Config for abilities
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UMSInputConfig> AbilityInputConfig;

	// Function to handle binding ability inputs (will be called from SetupPlayerInputComponent)
	void BindAbilityInput(UEnhancedInputComponent* EnhancedInputComponent);

	// Placeholder functions for handling ability input events (will be implemented later for specific abilities)

	// UFUNCTION() // Needs to be marked as UFUNCTION() for the binding to work
	// void AbilityInputTagPressed(FGameplayTag InputTag);
	//
	// UFUNCTION()
	// void AbilityInputTagReleased(FGameplayTag InputTag);

	UFUNCTION()
	void AbilityInputIDPressed(EAbilityInputID InputID);

	UFUNCTION()
	void AbilityInputIDReleased(EAbilityInputID InputID);
	
	
	// =======================
	// Attribute changes callback
	// =======================
protected:

	// Attribute change callbacks (optional to override for player-specific UI updates)
	// virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;
};
