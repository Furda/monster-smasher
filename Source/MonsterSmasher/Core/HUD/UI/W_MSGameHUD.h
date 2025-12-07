// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MSGameHUD.generated.h"

class UMSAttributeSet;
class UMSAbilitySystemComponent;
class UW_Vitals;
class UW_AbilitiesBar;

/**
 * Main HUD of the game
 */
UCLASS()
class MONSTERSMASHER_API UW_MSGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Called by PlayerController after widget is created */
	virtual void NativeOnInitialized() override;

	void InitializeHUDWithGAS(UMSAbilitySystemComponent* InASC, UMSAttributeSet* InAttributes) const;

	/** Expose access to sub-widgets */
	UFUNCTION(BlueprintCallable)
	UW_Vitals* GetVitalsWidget() const;

	UFUNCTION(BlueprintCallable)
	UW_AbilitiesBar* GetAbilitiesBarWidget() const;

protected:
	/** Health and stamina Bar reference (must match widget name in Blueprint) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UW_Vitals> Vitals;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UW_AbilitiesBar> AbilitiesBar;
};
