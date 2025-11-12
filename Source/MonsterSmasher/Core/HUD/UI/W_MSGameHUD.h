// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MSGameHUD.generated.h"

class UMSAttributeSet;
class UMSAbilitySystemComponent;
class UW_Vitals;
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

protected:
	/** Health Bar reference (must match widget name in Blueprint) */
	UPROPERTY(meta = (BindWidget))
	UW_Vitals* Vitals;
};
