// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayEffectTypes.h"
#include "W_StaminaBar.generated.h"

class UMSAbilitySystemComponent;
class UMSAttributeSet;
class UProgressBar;
class UTextBlock;

UCLASS()
class MONSTERSMASHER_API UW_StaminaBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	/** Initialize Stamina bar with ASC and attributes */
	UFUNCTION(BlueprintCallable)
	void InitializeWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes);

	/** Called when Stamina changes (bound to GAS attribute delegate) */
	void OnStaminaChanged(const FOnAttributeChangeData& Data);

	/** Updates the UI manually */
	void UpdateStaminaBar(float CurrentStamina, float MaxStamina);

protected:
	/** Progress bar for visual Stamina */
	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

	/** Text value of Stamina */
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* StaminaText;

private:
	UPROPERTY()
	TObjectPtr<UMSAbilitySystemComponent> CachedASC;
	UPROPERTY()
	TObjectPtr<UMSAttributeSet> CachedAttributes;
};
