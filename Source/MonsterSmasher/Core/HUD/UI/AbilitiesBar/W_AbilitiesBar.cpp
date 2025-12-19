// Fill out your copyright notice in the Description page of Project Settings.



#include "W_AbilitiesBar.h"

#include "Abilities/GameplayAbilityTypes.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Abilities/MSGameplayAbility.h"
#include "AbilitySlot/W_AbilitySlot.h"
#include "Components/HorizontalBox.h"


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

	if (CachedASC)
	{
		CachedASC->OnAbilitiesChanged.RemoveDynamic(this, &UW_AbilitiesBar::HandleOnAbilitiesChanged);
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
	
	// cache references
	CachedASC = InASC;
    
	// Unbind first to prevent double-binding
	if (CachedASC->OnAbilitiesChanged.IsBound())
	{
		CachedASC->OnAbilitiesChanged.RemoveDynamic(this, &UW_AbilitiesBar::HandleOnAbilitiesChanged);
	}
	
	CachedASC->OnAbilitiesChanged.AddDynamic(this, &UW_AbilitiesBar::HandleOnAbilitiesChanged);
	
	// Initial refresh
	RefreshAbilitySlots();
}

void UW_AbilitiesBar::HandleOnAbilitiesChanged()
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] UW_AbilitiesBar::HandleOnAbilitiesChanged - CALLED!"),
		   GetWorld()->GetNetMode() == NM_ListenServer ? TEXT("SERVER") : TEXT("CLIENT"));
	
		RefreshAbilitySlots();
}

void UW_AbilitiesBar::RefreshAbilitySlots()
{
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
