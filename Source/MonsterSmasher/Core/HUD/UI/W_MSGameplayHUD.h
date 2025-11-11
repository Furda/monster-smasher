// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "W_MSGameplayHUD.generated.h"

class UW_HealthBar;
/**
 * 
 */
UCLASS()
class MONSTERSMASHER_API UW_MSGameplayHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Called by PlayerController after widget is created */
	virtual void NativeOnInitialized() override;

	/** Expose access to sub-widgets if needed */
	UFUNCTION(BlueprintCallable)
	UW_HealthBar* GetHealthBarWidget() const;

protected:
	/** Health Bar reference (must match widget name in Blueprint) */
	UPROPERTY(meta = (BindWidget))
	UW_HealthBar* HealthBar;
};
