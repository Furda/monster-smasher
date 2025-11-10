// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HUDManagerComponent.generated.h"

class UMSAttributeSet;
class UMSAbilitySystemComponent;
class UW_MSHUD;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MONSTERSMASHER_API UHUDManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHUDManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	/** Call to initialize HUD once ASC and HUD are ready */
	void InitializeHUD(UMSAbilitySystemComponent* ASC, UMSAttributeSet* Attributes);

	/** Reference to the HUD widget */
	UPROPERTY()
	UW_MSHUD* GameHUD;

private:
	/** Tracks if initialization was already done */
	bool bIsHUDInitialized = false;

	/** Internal helper to trigger HUD setup */
	void SetupHUDInternal();

	/** Cached ASC and AttributeSet */
	UPROPERTY()
	UMSAbilitySystemComponent* CachedASC;

	UPROPERTY()
	UMSAttributeSet* CachedAttributes;

	/** Try Setup Timer (if things aren't ready yet) */
	FTimerHandle RetryInitTimerHandle;
};
