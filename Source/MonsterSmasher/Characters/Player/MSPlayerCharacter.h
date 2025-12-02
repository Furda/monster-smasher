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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UWeaponManagerComponent> WeaponManager;

	// =======================
	// Basic movement set up
	// =======================

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
	virtual UMSAbilitySystemComponent* GetMSAbilitySystemComponent() const override;

	// Return AttributeSet from the PlayerState
	virtual UMSAttributeSet* GetAttributeSet() const override;

protected:
	virtual void InitAbilitySystemAndAttributes() override;
	virtual void GrantStartingAbilities() override;


	// =======================
	// Ability Binding
	// =======================

	// Input Config for abilities
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UMSInputConfig> AbilityInputConfig;


	// =======================
	// Attribute changes callback
	// =======================

	// Attribute change callbacks (optional to override for player-specific UI updates)
	// virtual void OnHealthChanged(const FOnAttributeChangeData& Data) override;
};
