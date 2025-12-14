// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayTagContainer.h"
#include "GameplayTags/MyNativeGameplayTags.h"
#include "W_AbilitiesBar.generated.h"

class UMSGameplayAbility;
class UMSAbilitySystemComponent;
struct FGameplayEventData;
class UHorizontalBox;

/**
 * 
 */
UCLASS()
class MONSTERSMASHER_API UW_AbilitiesBar : public UUserWidget
{
	
	GENERATED_BODY()


	// =======================
	// Basic set up and overrides
	// =======================

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Initialize widget with ASC
	UFUNCTION(BlueprintCallable)
	void InitializeWithGAS(UMSAbilitySystemComponent* InASC);


	// =======================
	// Update abilities on event change
	// =======================

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	FGameplayTag AbilitiesChangedEventTag = TAG_Event_Abilities_Changed;
	FDelegateHandle AbilitiesChangedEventHandle;
	
	void HandleGameplayEvent(const FGameplayTag EventTag, const FGameplayEventData* Payload);
	
	// Refresh ability slots with abilities granted
	UFUNCTION(BlueprintCallable)
	void RefreshAbilitySlots();


	// =======================
	// Widget properties
	// =======================

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> AbilitySlotsContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Abilities")
	TSubclassOf<UUserWidget> AbilitySlotWidgetClass;

	// =======================
	// GAS properties
	// =======================

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UMSGameplayAbility>> AbilitiesToDisplay;

private:
	UPROPERTY()
	TObjectPtr<UMSAbilitySystemComponent> CachedASC;
};
