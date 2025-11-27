// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MSCharacterBase.generated.h"

class UHealthBarComponent;
class UGameplayAbility;
class UGameplayEffect;
class UMSAbilitySystemComponent;
class UMSAttributeSet;

UCLASS(Abstract) // Make this class abstract as it should not be instantiated directly
class MONSTERSMASHER_API AMSCharacterBase : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
	// =======================
	// Set up and overrides
	// =======================
	
public:
	AMSCharacterBase();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the character is possessed (server-side, or on client after initial setup)
	virtual void PossessedBy(AController* NewController) override;

	// Called on clients when the PlayerState is replicated
	virtual void OnRep_PlayerState() override;
	
	// =======================
	// GAS Set up and overrides
	// =======================
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UMSAttributeSet* GetAttributeSet() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UMSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "GAS | Attributes")
	TObjectPtr<UMSAttributeSet> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	// TODO: Check if this property is needed
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;

	// Flag to ensure InitAbilityActorInfo and ApplyStartupGAS are called only once.
	UPROPERTY(Transient) // Transient: Not saved to disk, cleared on load
	bool bAbilitiesInitialized = false;

	// GAS init helper
	virtual void InitAbilitySystemAndAttributes();

	// Character-specific startup GAS application
	// i.e. Grant Starting Attributes and Abilities...
	virtual void ApplyStartupGAS();
	
	// =======================
	// Ability Granting (to be overridden by derived classes)
	// =======================

	// Grant Default/Starting attributes to the character
	virtual void GrantStartingAttributes();

	// Grant Default/Starting abilities to the character
	virtual void GrantStartingAbilities();
	
	// =======================
	// Attribute Change Callbacks (to be overridden by derived classes)
	// =======================

	// virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
};
