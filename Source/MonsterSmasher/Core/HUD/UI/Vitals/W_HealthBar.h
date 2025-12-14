// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_HealthBar.generated.h"

class UMSAbilitySystemComponent;
class UMSAttributeSet;
class UProgressBar;
class UTextBlock;
struct FOnAttributeChangeData;

UCLASS()
class MONSTERSMASHER_API UW_HealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** Initialize Health bar with ASC and attributes */
	UFUNCTION(BlueprintCallable)
	void InitializeWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes);

	/** Called when health changes (bound to GAS attribute delegate) */
	void OnHealthChanged(const FOnAttributeChangeData& Data);

	/** Updates the UI manually */
	void UpdateHealthBar(float CurrentHealth, float MaxHealth) const;

protected:
	/** Progress bar for visual health */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	/** Text value of health */
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> HealthText;

private:
	UPROPERTY()
	TObjectPtr<UMSAbilitySystemComponent> CachedASC;
	UPROPERTY()
	TObjectPtr<UMSAttributeSet> CachedAttributes;
};
