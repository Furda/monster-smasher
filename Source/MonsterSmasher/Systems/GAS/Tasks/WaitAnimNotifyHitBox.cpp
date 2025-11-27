#include "WaitAnimNotifyHitBox.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotifies/ANS_HitBoxWindow.h"
#include "AnimNotifies/AN_HitBoxStart.h"

UWaitAnimNotifyHitBox* UWaitAnimNotifyHitBox::WaitForHitBoxNotify(
	UGameplayAbility* OwningAbility,
	FName TaskInstanceName,
	TSubclassOf<UAnimNotify> InNotifyClass,
	UAnimMontage* InMontage)
{
	UWaitAnimNotifyHitBox* MyTask = NewAbilityTask<UWaitAnimNotifyHitBox>(OwningAbility, TaskInstanceName);
	MyTask->NotifyClassToWaitFor = InNotifyClass;
	MyTask->MontageToWatch = InMontage;
	return MyTask;
}

void UWaitAnimNotifyHitBox::Activate()
{
	Super::Activate();

	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character || !MontageToWatch)
	{
		OnAbilityCancelled.Broadcast();
		EndTask();
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (!AnimInstance)
	{
		OnAbilityCancelled.Broadcast();
		EndTask();
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AnimInstance Bound = %s"), *AnimInstance->GetClass()->GetName());

	// Bind dynamic (UObject-friendly)
	// Bind OnMontageNotifyFired to the AnimInstance's notify delegate
	AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(
		this, &UWaitAnimNotifyHitBox::OnMontageNotifyFired);

	// Cache the instance for safe unbinding later
	BoundAnimInstance = AnimInstance;

	// Bind to the ability cancel event for cleanup
	if (AbilitySystemComponent.IsValid())
	{
		AbilitySystemComponent->OnAbilityEnded.AddUObject(this, &UWaitAnimNotifyHitBox::OnAbilityCanceled);
	}
}

void UWaitAnimNotifyHitBox::OnDestroy(bool AbilityEnded)
{
	if (BoundAnimInstance.IsValid())
	{
		// Unbind from the AnimInstance delegate
		BoundAnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(
			this, &UWaitAnimNotifyHitBox::OnMontageNotifyFired);
	}
	// Fallback if AnimInstance is not cached
	else if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
	{
		if (UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnPlayMontageNotifyBegin.RemoveDynamic(
				this, &UWaitAnimNotifyHitBox::OnMontageNotifyFired);
		}
	}

	if (AbilitySystemComponent.IsValid())
	{
		// Unbind from the ability cancel event
		AbilitySystemComponent->OnAbilityEnded.RemoveAll(this);
	}

	Super::OnDestroy(AbilityEnded);
}

// Corrected function signature
void UWaitAnimNotifyHitBox::OnMontageNotifyFired(FName NotifyName,
                                                 const FBranchingPointNotifyPayload& Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("UWaitAnimNotifyHitBox::OnMontageNotifyFired called. Notify=%s"),
	       *NotifyName.ToString());

	const FAnimNotifyEvent* Event = Payload.NotifyEvent;
	if (!Event)
	{
		return;
	}

	const UObject* NotifyObject = Event->Notify;
	if (!NotifyObject)
	{
		return;
	}

	// Class-based match (best practice)
	if (!NotifyObject->IsA(NotifyClassToWaitFor))
	{
		return;
	}

	// ✔ Extract GameplayTags from the notify class (if it has them)
	FGameplayTagContainer ExtractedTags;

	if (const UAN_HitBoxStart* HitStart = Cast<UAN_HitBoxStart>(NotifyObject))
	{
		ExtractedTags.AddTag(HitStart->HitBoxTag);
	}
	else if (const UANS_HitBoxWindow* HitWindow = Cast<UANS_HitBoxWindow>(NotifyObject))
	{
		ExtractedTags.AddTag(HitWindow->HitBoxTag);
	}

	// Broadcast with tags (extend your delegate for tags if you want)
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnHitBoxWindowOpen.Broadcast(); // or pass tags / strength
	}

	EndTask();
}

void UWaitAnimNotifyHitBox::OnAbilityCanceled(const FAbilityEndedData& AbilityEndedData)
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnAbilityCancelled.Broadcast();
	}
	EndTask();
}
