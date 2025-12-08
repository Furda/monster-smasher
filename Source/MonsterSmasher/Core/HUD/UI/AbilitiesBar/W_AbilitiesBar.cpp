// Fill out your copyright notice in the Description page of Project Settings.



#include "W_AbilitiesBar.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "AbilitySlot/W_AbilitySlot.h"
#include "Components/HorizontalBox.h"
#include "EditorState/EditorState.h"
#include "Trace/Analysis.h"


void UW_AbilitiesBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UW_AbilitiesBar::NativeDestruct()
{
	if (CachedASC && AbilitiesChangedEventHandle.IsValid())
	{
		FGameplayTagContainer EventTags;
		EventTags.AddTag(AbilitiesChangedEventTag);
		CachedASC->RemoveGameplayEventTagContainerDelegate(EventTags, AbilitiesChangedEventHandle);
		AbilitiesChangedEventHandle.Reset();
	}
	Super::NativeDestruct();
}

void UW_AbilitiesBar::InitializeWithGAS(UMSAbilitySystemComponent* InASC)
{
	if (!InASC)
	{
		return;
	}
	if (!AbilitySlotWidgetClass || !AbilitySlotsContainer)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UW_AbilitiesBar::InitializeWithGAS"));
	
	// cache references
	CachedASC = InASC;

	// Bind to gameplay event with a specific tag you want to listen for:
	FGameplayTagContainer EventTags;
	EventTags.AddTag(AbilitiesChangedEventTag);
	AbilitiesChangedEventHandle = CachedASC->AddGameplayEventTagContainerDelegate(
		EventTags,
		FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(
			this, &UW_AbilitiesBar::HandleGameplayEvent
		)
	);
	
	// Refresh abilities bar
	RefreshAbilitySlots();
}

void UW_AbilitiesBar::HandleGameplayEvent(const FGameplayTag EventTag, const FGameplayEventData* Payload)
{
	UE_LOG(LogTemp, Warning, TEXT("UW_AbilitiesBar::HandleGameplayEvent: Received Gameplay Event: %s"),
	       *EventTag.ToString());
	
	if (EventTag == AbilitiesChangedEventTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("UW_AbilitiesBar::HandleGameplayEvent: Refreshing abilities bar..."));
		RefreshAbilitySlots();
	}
}

void UW_AbilitiesBar::RefreshAbilitySlots()
{
	UE_LOG(LogTemp, Warning, TEXT("UW_AbilitiesBar::RefreshAbilitySlots"));
	
	AbilitySlotsContainer->ClearChildren();

	// Get all granted abilities
	TArray<FGameplayAbilitySpecHandle> GrantedAbilities;
	CachedASC->GetAllAbilities(GrantedAbilities);
	

	for (const FGameplayAbilitySpecHandle AbilitySpecHandle : GrantedAbilities)
	{
		if (!AbilitySpecHandle.IsValid())
		{
			continue;
		}
		
		UMSGameplayAbility* Ability = Cast<UMSGameplayAbility>(
			CachedASC->FindAbilitySpecFromHandle(AbilitySpecHandle)->Ability);
		
		// Exclude abilities that should not be in the Abilities Bar
		if (!Ability || !Ability->bShouldShowInAbilitiesBar) continue;
		
		// Set up ability slot widget
		UW_AbilitySlot* AbilitySlotWidget = CreateWidget<UW_AbilitySlot>(GetWorld(), AbilitySlotWidgetClass);
		
		if (!AbilitySlotWidget) continue;
		
		AbilitySlotWidget->InitializeWithGAS(CachedASC, AbilitySpecHandle);
		AbilitySlotsContainer->AddChild(AbilitySlotWidget);
	}
}
