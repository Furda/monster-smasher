// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MSAbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilitiesChangedSignature);

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

public:
	
	virtual void AbilityLocalInputPressed(int32 InputID) override;
	virtual void AbilityLocalInputReleased(int32 InputID) override;

	
	// ==============================
	// Granting abilities
	// ==============================
	
public:
	
	UPROPERTY(BlueprintAssignable, Category = "Abilities Events")
	FOnAbilitiesChangedSignature OnAbilitiesChanged;
	
	// A helper to broadcast the change
	void BroadcastAbilitiesChanged() const;
	
	// Function to give abilities based on a data asset
	TArray<FGameplayAbilitySpecHandle> GiveAbilitiesFromInputConfig(const class UMSInputConfig* InputConfig,
	                                                                AActor* InOwnerActor);
protected:
	
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Abilities")
	TArray<FGameplayAbilitySpec> LastActivatableAbilities;
	
	virtual void OnRep_ActivateAbilities() override;
	virtual void OnGiveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	virtual void OnRemoveAbility(FGameplayAbilitySpec& AbilitySpec) override;
	
	void ScheduleDeferredBroadcast();
	void ExecuteDeferredBroadcast();

private:
	FTimerHandle DeferredBroadcastTimer;
	int32 PendingAbilityChanges = 0;
};
