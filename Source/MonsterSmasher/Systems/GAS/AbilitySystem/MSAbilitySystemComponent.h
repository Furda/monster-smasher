// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MSAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MONSTERSMASHER_API UMSAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMSAbilitySystemComponent();

	// Called every frame to allow processing
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//                            FActorComponentTickFunction* ThisTickFunction) override;

	// These are overrides of internal UAbilitySystemComponent functions.
	// They are called by the base ASC's internal logic or our ProcessAbilityInput.
	// virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	// virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

	// These are public as they're called from the Character's input binding
	// Add custom methods for tag-based input.
	// void AbilityInputTagPressed(const FGameplayTag& InputTag);
	// void AbilityInputTagReleased(const FGameplayTag& InputTag);
	
	void AbilityLocalInputPressed(int32 InputID) override;
	void AbilityLocalInputReleased(int32 InputID) override;

	// Control input processing: Process spec handles
	// It will be called from TickComponent.
	// void ProcessAbilityInput(float DeltaTime, bool bGamePaused);

	// Function to give abilities based on a data asset
	void GiveAbilitiesFromInputConfig(const class UMSInputConfig* InputConfig, AActor* InOwnerActor);

	// // These are the custom lists that Lyra uses to track abilities that had their input pressed/released.
	// // They are populated in AbilityInputTagPressed/Released and consumed in ProcessAbilityInput.
	// // They store FGameplayAbilitySpecHandle (a lightweight identifier for a granted ability).
	// TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	// TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	// TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	//
	// // Internal helper to process a single ability spec's input
	// // These helper functions from Lyra are for dynamically binding/unbinding abilities,
	// // often for UI abilities or abilities granted/removed on the fly.
	// // For now, you might not need to implement them fully until you get to advanced scenarios.
	// // void ProcessAbilityInput_BindTagToAbility(FGameplayAbilitySpec& Spec);
	// // void ProcessAbilityInput_ClearBinding(FGameplayAbilitySpec& Spec);
	//
	// virtual void Deactivate() override;
	// virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
