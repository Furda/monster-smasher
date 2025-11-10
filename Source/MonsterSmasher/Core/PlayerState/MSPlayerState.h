// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerState.h"
#include "AbilitySystemInterface.h"
#include "MSPlayerState.generated.h"

class UMSAbilitySystemComponent;
class UMSAttributeSet;

/**
 * 
 */
UCLASS()
class MONSTERSMASHER_API AMSPlayerState : public AModularPlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMSPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GAS")
	UMSAttributeSet* GetAttributeSet() const;

	// Called by the PlayerState/GameMode to initialize this character's GAS components
	// This function should *not* directly initialize AbilityActorInfo for PlayerState-owned ASCs
	// It's meant to get the ASC from the owner and set it as the character's reference
	void InitAbilitySystemAndAttributes();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UMSAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	TObjectPtr<UMSAttributeSet> AttributeSet;

	// Helper to store the avatar actor, for InitAbilityActorInfo
	UPROPERTY()
	TObjectPtr<AActor> CachedAvatarActor;

	// Removed BeginPlay, as PostInitializeComponents is more appropriate for GAS setup
	// virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
};
