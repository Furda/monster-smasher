// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularCharacter.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MSCharacterBase.generated.h"

struct FGameplayAbilitySpecHandle;
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
	virtual UMSAbilitySystemComponent* GetMSAbilitySystemComponent() const;
	virtual UMSAttributeSet* GetAttributeSet() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UMSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "GAS | Attributes")
	TObjectPtr<UMSAttributeSet> AttributeSet;

	// Flag to ensure InitAbilityActorInfo and ApplyStartupGAS are called only once.
	UPROPERTY(Transient) // Transient: Not saved to disk, cleared on load
	bool bAbilitiesInitialized = false;

	// GAS init helper
	virtual void InitAbilitySystemAndAttributes();


	// =======================
	// Ability Granting (to be overridden by derived classes)
	// =======================
public:
	virtual TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);
	virtual void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Attributes")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	// TODO: Change to ability sets
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GAS | Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;

	UPROPERTY(BlueprintReadOnly, Category = "GAS | Abilities")
	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;

	// Grant Default/Starting attributes to the character
	virtual void GrantStartingAttributes();

	// TODO: Change logic of granting and removing abilities to ability sets
	virtual void GrantStartingAbilities();
	virtual void SendAbilitiesChangedEvent();


	// =======================
	// Attribute Change Callbacks (to be overridden by derived classes)
	// =======================

	// virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
};
