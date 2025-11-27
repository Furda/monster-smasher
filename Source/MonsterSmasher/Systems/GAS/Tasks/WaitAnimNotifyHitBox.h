// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "WaitAnimNotifyHitBox.generated.h"

// Delegate signature for when the specific notify fires
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHitBoxNotifyDelegate);

/**
 * Custom ability task to wait for a specific Anim Notify to fire during the active
 * Montage. This is used to precisely time when a hit-box should be active for damage calculation.
 */
UCLASS()
class MONSTERSMASHER_API UWaitAnimNotifyHitBox : public UAbilityTask
{
	GENERATED_BODY()

public:
	// Output delegate fired when the required Anim Notify fires in the Montage.
	UPROPERTY(BlueprintAssignable)
	FOnHitBoxNotifyDelegate OnHitBoxWindowOpen;

	// Output delegate fired if the ability is cancelled before the notify fires.
	UPROPERTY(BlueprintAssignable)
	FOnHitBoxNotifyDelegate OnAbilityCancelled;

	/**
	* Creates a task that waits for a specific Anim Notify to fire.
	* @param TaskInstanceName Name of the task for debugging.
	* @param NotifyName The name of the Anim Notify to wait for (e.g., "HitBoxStart").
	* @param InMontage The montage that contains the notify.
	* @return The new Ability Task instance.
	*/
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UWaitAnimNotifyHitBox* WaitForHitBoxNotify(
		UGameplayAbility* OwningAbility,
		FName TaskInstanceName,
		TSubclassOf<UAnimNotify> InNotifyClass,
		UAnimMontage* InMontage
	);

private:
	// The Anim Notify Class we are waiting for. Supports both UAnimNotify and UAnimNotifyState
	UPROPERTY()
	TSubclassOf<UObject> NotifyClassToWaitFor;

	// The Montage we are looking for the notify in.
	UPROPERTY()
	UAnimMontage* MontageToWatch;

	// Begin UAbilityTask interface
	virtual void Activate() override;
	virtual void OnDestroy(bool AbilityEnded) override;
	// End UAbilityTask interface

	// Cached anim instance
	TWeakObjectPtr<UAnimInstance> BoundAnimInstance;

	// Function to bind to the Anim Instance's internal delegate for this specific notify
	UFUNCTION()
	void OnMontageNotifyFired(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

	// Called when the ability is cancelled.
	UFUNCTION()
	void OnAbilityCanceled(const FAbilityEndedData& AbilityEndedData);
};
