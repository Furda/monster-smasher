// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_Vitals.generated.h"

class UW_HealthBar;
class UW_StaminaBar;
class UMSAbilitySystemComponent;
class UMSAttributeSet;

/**
 * Contains the vital stats UI elements (health, stamina, etc.)
 */
UCLASS()
class MONSTERSMASHER_API UW_Vitals : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Called by PlayerController after widget is created */
	virtual void NativeOnInitialized() override;

	void InitializeWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes) const;

	/** Expose access to sub-widgets */
	UFUNCTION(BlueprintCallable)
	UW_HealthBar* GetHealthBarWidget() const;

	UFUNCTION(BlueprintCallable)
	UW_StaminaBar* GetStaminaBarWidget() const;

protected:
	/** Health Bar reference (must match widget name in Blueprint) */
	UPROPERTY(meta = (BindWidget))
	UW_HealthBar* HealthBar;

	/** Stamina Bar reference (must match widget name in Blueprint) */
	UPROPERTY(meta = (BindWidget))
	UW_StaminaBar* StaminaBar;
};
