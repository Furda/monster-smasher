// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "W_AbilitySlot.generated.h"


class UMSAbilitySystemComponent;
class UMSGameplayAbility;
class UOverlay;
class UTextBlock;
class UImage;

/**
 *  This class represents a Ability slot inside the Ability bar widget
 */
UCLASS()
class MONSTERSMASHER_API UW_AbilitySlot : public UUserWidget
{
	GENERATED_BODY()


	// =======================
	// Basic set up and overrides
	// =======================

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Initialize widget with ASC
	// This function is supposed to be called from the Abilities Bar widget
	UFUNCTION(BlueprintCallable)
	void InitializeWithGAS(UMSAbilitySystemComponent* InASC, FGameplayAbilitySpecHandle InAbility);


	// =======================
	// Widget properties
	// =======================

protected:
	// Icon for the ability represented by this Ability slot
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> AbilityIcon;

	// Text value for the name of the ability. This will show if the Ability icon is not set
	// UPROPERTY(meta = (BindWidgetOptional))
	// TObjectPtr<UTextBlock> AbilityText;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> CooldownText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> CooldownOverlay;


	// =======================
	// Cooldowns: Actions and events
	// =======================

public:
	UFUNCTION(BlueprintCallable)
	void OnCooldownStarted();
	UFUNCTION(BlueprintCallable)
	void UpdateCooldown();
	UFUNCTION(BlueprintCallable)
	void OnCooldownEnded();
	
protected:
	// Called when any gameplay effect is added to the owner ASC
	UFUNCTION(BlueprintCallable)
	void OnGameplayEffectAdded(UAbilitySystemComponent* Target, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveHandle);
	

	UPROPERTY()
	FTimerHandle CooldownTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category="Cooldown")
	float CooldownUpdateRate = 1.0f;


	// =======================
	// GAS properties
	// =======================

private:
	UPROPERTY()
	TObjectPtr<UMSAbilitySystemComponent> CachedASC;
	UPROPERTY()
	FGameplayAbilitySpecHandle CachedAbilitySpecHandle;
	UPROPERTY()
	TObjectPtr<UMSGameplayAbility> CachedAbility;
	UPROPERTY()
	FGameplayTag CooldownTagToWatch;
	
};
