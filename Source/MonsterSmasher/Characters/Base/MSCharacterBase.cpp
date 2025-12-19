// Fill out your copyright notice in the Description page of Project Settings.



#include "MSCharacterBase.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"
#include "GameplayEffectTypes.h"
#include "GameplayTags/MyNativeGameplayTags.h"
#include "Input/FMSInputAction.h"


// =====================================================================
// Set up and overrides
// =====================================================================

AMSCharacterBase::AMSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true; // Character can tick
}

void AMSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMSCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// SERVER ONLY: Initialize GAS for the server's PlayerState and Character
	// This handles the server-side authoritative setup.
	InitAbilitySystemAndAttributes();
}

void AMSCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// CLIENT ONLY: Initialize GAS for the client's PlayerState and Character
	// This is called when the client's PlayerState is replicated and ready.
	InitAbilitySystemAndAttributes();
}


// =====================================================================
// GAS Set up and overrides
// =====================================================================

UAbilitySystemComponent* AMSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMSAbilitySystemComponent* AMSCharacterBase::GetMSAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMSAttributeSet* AMSCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

// Init Ability System Component and attributes for the character
void AMSCharacterBase::InitAbilitySystemAndAttributes()
{
	// This function is for the character to *get a reference* to its ASC,
	// and set up AbilityActorInfo if it has not been initialized.

	// If this character instance's abilities are already initialized, do nothing.
	if (bAbilitiesInitialized)
	{
		return;
	}

	// Assign the character's inherited AbilitySystemComponent and AttributeSet pointers
	// These will be overridden in derived classes to fetch from PlayerState or a created component.
	AbilitySystemComponent = Cast<UMSAbilitySystemComponent>(GetAbilitySystemComponent());
	AttributeSet = GetAttributeSet();

	if (!AbilitySystemComponent || !AttributeSet)
	{
		UE_LOG(LogTemp, Error,
		       TEXT(
			       "AMSCharacterBase::InitAbilitySystemAndAttributes - GAS components are null AFTER CAST! Casted ASC is: %s, AttributeSet is: %s. Owner: %s, Avatar: %s"
		       ),
		       *GetNameSafe(AbilitySystemComponent),
		       *GetNameSafe(AttributeSet),
		       *GetNameSafe(GetOwner()),
		       *GetNameSafe(this)
		);
		return;
	}

	// Check if InitAbilityActorInfo needs to be called.
	// It should be called once per actor on both server and client.
	if (AbilitySystemComponent->GetAvatarActor() != this)
	{
		AbilitySystemComponent->InitAbilityActorInfo(AbilitySystemComponent->GetOwnerActor(), this);

		// This flag ensures ApplyStartupGAS and future calls don't re-initialize.
		bAbilitiesInitialized = true;
	}
	else
	{
		// UE_LOG(LogTemp, Log,
		//        TEXT("AMSCharacterBase: InitAbilityActorInfo skipped, AvatarActor already set. Owner: %s, Avatar: %s"),
		//        *GetNameSafe(AbilitySystemComponent->GetOwnerActor()), *GetNameSafe(this));
	}

	// Apply default attributes and grant startup abilities.
	// This should ONLY happen on the server, and only once.
	if (HasAuthority() && AbilitySystemComponent && AbilitySystemComponent->GetAvatarActor() == this)
	{
		GrantStartingAttributes();
		GrantStartingAbilities();
	}
}


// =====================================================================
// Ability Granting
// =====================================================================

TArray<FGameplayAbilitySpecHandle> AMSCharacterBase::GrantAbilities(
	TArray<FMSInputAction> AbilitiesToGrant, UObject* SourceObject)
{
	if (!AbilitySystemComponent || !HasAuthority())
	{
		return TArray<FGameplayAbilitySpecHandle>();
	}

	TArray<FGameplayAbilitySpecHandle> GrantedAbilitiesHandles;
	for (const FMSInputAction& AbilityConfig : AbilitiesToGrant)
	{
		if (!AbilityConfig.AbilityClass) continue;
		
		// Set ability spec (Associate tag, input ID, SourceObject)
		int32 AbilityInputID = AbilityConfig.InputID == EAbilityInputID::None ? -1 : static_cast<int32>(AbilityConfig.InputID);
		FGameplayAbilitySpec NewAbilitySpec(AbilityConfig.AbilityClass, 1, AbilityInputID);
		if (AbilityConfig.InputTag.IsValid()) NewAbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityConfig.InputTag);
		if (SourceObject) NewAbilitySpec.SourceObject = SourceObject;
		
		FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(NewAbilitySpec);
		GrantedAbilitiesHandles.Add(AbilitySpecHandle);
	}

	UE_LOG(LogTemp, Warning, TEXT("Granted abilities: %d"), GrantedAbilitiesHandles.Num());
	
	return GrantedAbilitiesHandles;
}


void AMSCharacterBase::RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove)
{
	if (!AbilitySystemComponent || !HasAuthority())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle AbilitySpecHandle : AbilitiesToRemove)
	{
		AbilitySystemComponent->ClearAbility(AbilitySpecHandle);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Removed abilities: %d"), AbilitiesToRemove.Num());
}



// Grant Default/Starting attributes to the character
void AMSCharacterBase::GrantStartingAttributes()
{
	// Base implementation for granting starting attributes.
	// Derived classes can call Super::GrantStartingAttributes() or override completely.

	// Grant default attributes if the ASC is valid and is in the server
	if (!AbilitySystemComponent || !HasAuthority() || !DefaultAttributesEffect)
	{
		if (!DefaultAttributesEffect)
		{
			UE_LOG(LogTemp, Warning,
			       TEXT("AMSCharacterBase::GrantStartingAttributes - DefaultAttributesEffect is null for %s!"),
			       *GetNameSafe(this));
		}
		return;
	}

	// Apply Effect to init attributes
	FGameplayEffectContextHandle Context = AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(
		DefaultAttributesEffect, 1.f, Context);
	if (SpecHandle.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	}
}


// This method offer general functionality, but derived classes should override this method
void AMSCharacterBase::GrantStartingAbilities()
{
	if (!AbilitySystemComponent || !HasAuthority())
	{
		return;
	}

	if (StartingAbilities.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AMSCharacterBase::GrantStartingAbilities - StartingAbilities is empty!"));
		return;
	}

	for (const FMSInputAction& AbilityConfig : StartingAbilities)
	{
		if (!AbilityConfig.AbilityClass) continue;
	

		// Set ability spec (Associate tag, input ID, SourceObject)
		int32 AbilityInputID = AbilityConfig.InputID == EAbilityInputID::None ? -1 : static_cast<int32>(AbilityConfig.InputID);
		FGameplayAbilitySpec NewAbilitySpec(AbilityConfig.AbilityClass, 1, AbilityInputID);
		if (AbilityConfig.InputTag.IsValid()) NewAbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilityConfig.InputTag);
		NewAbilitySpec.SourceObject = this;
		
		FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystemComponent->GiveAbility(NewAbilitySpec);
		GrantedAbilities.Add(AbilitySpecHandle);
	}
}


