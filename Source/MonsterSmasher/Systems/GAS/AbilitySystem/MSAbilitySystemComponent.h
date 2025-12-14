// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MSAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MONSTERSMASHER_API UMSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	// ==============================
	// Basic set up and overrides
	// ==============================

public:
	// Sets default values for this component's properties
	UMSAbilitySystemComponent();


	// ==============================
	// Input abilities events
	// ==============================

	virtual void AbilityLocalInputPressed(int32 InputID) override;
	virtual void AbilityLocalInputReleased(int32 InputID) override;

	// ==============================
	// Granting abilities
	// ==============================
	
	// Function to give abilities based on a data asset
	TArray<FGameplayAbilitySpecHandle> GiveAbilitiesFromInputConfig(const class UMSInputConfig* InputConfig,
	                                                                AActor* InOwnerActor);
protected:
	
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Abilities")
	TArray<FGameplayAbilitySpec> LastActivatableAbilities;
	
	virtual void OnRep_ActivateAbilities() override;
};
