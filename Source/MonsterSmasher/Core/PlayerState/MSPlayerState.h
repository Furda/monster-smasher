// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "MSPlayerState.generated.h"

class UMSAbilitySystemComponent;
class UMSAttributeSet;

/**
 * @brief A subclass of AModularPlayerState that incorporates Gameplay Ability System (GAS) and attributes.
 *
 * AMSPlayerState manages the integration of the GAS framework with player-specific functionality, such as
 * storing and managing Gameplay Ability System Components and Attribute Sets. It supports multiplayer compatibility
 * and properly initializes GAS-related functionalities during PlayerState lifecycle events.
 *
 * This class also provides hooks for attribute change callbacks, enabling derived classes to handle
 * attribute-specific logic.
 */
UCLASS()
class MONSTERSMASHER_API AMSPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// =======================
	// Set up and overrides
	// =======================
public:
	AMSPlayerState();
	
protected:
	// Removed BeginPlay, as PostInitializeComponents is more appropriate for GAS setup
	// virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	
	
	// =======================
	// GAS Set up and overrides
	// =======================
	
public:

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GAS")
	UMSAttributeSet* GetAttributeSet() const;

	// This function should *not* directly initialize AbilityActorInfo for PlayerState-owned ASCs
	// It's meant to get the ASC from the owner and set it as the character's reference
	void InitAbilitySystemAndAttributes();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UMSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UMSAttributeSet> AttributeSet;

	// Helper to store the avatar actor, for InitAbilityActorInfo
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<AActor> CachedAvatarActor;
	
	
	// =======================
	// Attribute Change Callbacks
	// =======================
	
protected:
	
	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
};
