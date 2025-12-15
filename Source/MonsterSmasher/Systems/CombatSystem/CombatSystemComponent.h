// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Systems/WeaponSystem/Data/FCombatAbilityData.h" // Need the complete type since it is being used in a TMap

#include "CombatSystemComponent.generated.h"

class UWeaponDataAsset;
class UMSAttributeSet;
class UMSAbilitySystemComponent;

// Define delegates 
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatDataUpdated);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MONSTERSMASHER_API UCombatSystemComponent : public UActorComponent
{
	GENERATED_BODY()

	
	// ==================================
	// Setup and overrides
	// ==================================
	
public:
	UCombatSystemComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TWeakObjectPtr<UMSAbilitySystemComponent> CachedASC;

	UPROPERTY()
	TWeakObjectPtr<UMSAttributeSet> CachedAttributeSet;
	
	// ====================================================
	// Weapon data management
	// ====================================================
	
public:
	
	// Event broadcast when the weapon changes
	UPROPERTY(BlueprintAssignable, Category = "Combat Events")
	FOnCombatDataUpdated OnCombatDataUpdated;
	
	// Called by Weapon System when the weapon changes
	UFUNCTION(BlueprintCallable, Category = "Combat")
	void UpdateCombatData(UWeaponDataAsset* NewWeaponData);

	// Returns ability configuration data for the specified ability tag
	// C++ only - fast pointer access
	const FCombatAbilityData* GetAbilityData(FGameplayTag AbilityTag) const;
	
	// Blueprint accessible - safe copy
	UFUNCTION(BlueprintCallable, Category = "Combat", 
		meta = (DisplayName = "Get Ability Data"))
	bool GetAbilityData(FGameplayTag AbilityTag, FCombatAbilityData& OutAbilityData) const;

protected:
	UPROPERTY()
	TObjectPtr<UWeaponDataAsset> CurrentWeaponData;

	// Cached lookup map for fast ability data retrieval
	UPROPERTY()
	TMap<FGameplayTag, FCombatAbilityData> CachedAbilityData;

	// Example tags: Combat.Ability.LightAttack, Combat.Ability.HeavyAttack
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Tags")
	FGameplayTag LightAttackTag;

	UPROPERTY(EditDefaultsOnly, Category = "Combat|Tags")
	FGameplayTag HeavyAttackTag;
	
	UPROPERTY(EditDefaultsOnly, Category = "Combat|Tags")
	FGameplayTag BlockTag;
	
	
	// ====================================================
	// Validation
	// ====================================================
	
public:
	
	// TODO: Check if we need failures tags
	// State management
	UFUNCTION(BlueprintCallable, Category = "Combat")
	bool CanActivateAbility(FGameplayTag AbilityTag, FGameplayTagContainer& OutFailureTags) const;
	
	
	// ====================================================
	// Combo System Management
	// ====================================================

public:
	
	// Called when light attack activates
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void RegisterComboAttack(FGameplayTag AttackTag);

	// Called by ability to get current combo animation
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	UAnimMontage* GetCurrentComboMontage(FGameplayTag AttackTag) const;

	// Check if player can progress combo
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	bool IsInComboWindow() const;

	// Reset combo (called on timeout or hit interrupt)
	UFUNCTION(BlueprintCallable, Category = "Combat|Combo")
	void ResetCombo();

protected:
	UPROPERTY()
	float LastComboInputTime = 0.f;

	UPROPERTY()
	int32 CurrentComboIndex = 0;
	
	UPROPERTY()
	FGameplayTag ActiveComboTag; // Currently active combo type

	UPROPERTY()
	FTimerHandle ComboResetTimerHandle;

	// TODO: Doble check logic and when it is called, Right now it is called on the timer update
	void OnComboWindowExpired();
	
	
	// ====================================================
	// TODO: Hit Detection & Response
	// ====================================================
	
	// UFUNCTION()
	// void OnWeaponHit(FHitResult HitResult);
	//
	// UFUNCTION()
	// void ProcessHitTarget(AActor* Target, const FGameplayEffectSpecHandle& DamageSpec);
};
