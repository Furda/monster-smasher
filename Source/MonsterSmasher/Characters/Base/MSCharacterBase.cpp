// Fill out your copyright notice in the Description page of Project Settings.


#include "MSCharacterBase.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"


AMSCharacterBase::AMSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true; // Character can tick
}

void AMSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent* AMSCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMSAttributeSet* AMSCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

void AMSCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// SERVER ONLY: Initialize GAS for the server's PlayerState and Character
	// This handles the server-side authoritative setup.
	InitAbilitySystemAndAttributes();

	// if (HasAuthority() && IsValid(NewController))
	// {
	// 	if (AMSPlayerState* PS = GetPlayerState<AMSPlayerState>())
	// 	{
	// 		// Set the AvatarActor in the PlayerState's ASC
	// 		PS->InitAbilitySystemAndAttributes();
	//
	// 		// Then set the character's local reference
	// 		InitAbilitySystemAndAttributes(); // This sets the character's AbilitySystemComponent pointer
	// 	}
	// }
}

void AMSCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// CLIENT ONLY: Initialize GAS for the client's PlayerState and Character
	// This is called when the client's PlayerState is replicated and ready.
	InitAbilitySystemAndAttributes();

	// if (AMSPlayerState* MSPS = GetPlayerState<AMSPlayerState>()) // GetPlayerState() handles client/server differences
	// {
	// 	// Set the AvatarActor in the PlayerState's ASC on the client
	// 	MSPS->InitAbilitySystemAndAttributes();
	//
	// 	// Then set the character's local reference
	// 	InitAbilitySystemAndAttributes(); // This sets the character's AbilitySystemComponent pointer
	// }
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
	// GetAvatarActor() == this: Means this character is already the avatar for this ASC.
	// This is the most reliable check to avoid redundant InitAbilityActorInfo calls.
	if (AbilitySystemComponent->GetAvatarActor() != this)
	{
		// The OwnerActor is the one that *owns* the ASC (e.g., PlayerState or the Character itself).
		// The AvatarActor is the one the abilities *act on* (e.g., the Character).
		AbilitySystemComponent->InitAbilityActorInfo(AbilitySystemComponent->GetOwnerActor(), this);

		// This flag ensures ApplyStartupGAS and future calls don't re-initialize.
		bAbilitiesInitialized = true;

		// UE_LOG(LogTemp, Warning, TEXT("AMSCharacterBase: InitAbilityActorInfo called. Owner: %s, Avatar: %s"),
		//        *GetNameSafe(AbilitySystemComponent->GetOwnerActor()), *GetNameSafe(this));
	}
	else
	{
		UE_LOG(LogTemp, Log,
		       TEXT("AMSCharacterBase: InitAbilityActorInfo skipped, AvatarActor already set. Owner: %s, Avatar: %s"),
		       *GetNameSafe(AbilitySystemComponent->GetOwnerActor()), *GetNameSafe(this));
	}

	// ----- Bind attributes are in the ASC Owner (PlayerState class) -----
	
	// Bind attribute change delegates on the ASC to character's callbacks.
	// This is done on both server and client, as clients need UI updates.
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(
	// 	this, &AMSCharacterBase::OnHealthChanged);
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AMSCharacterBase::OnMaxHealthChanged);
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AMSCharacterBase::OnStaminaChanged);
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &AMSCharacterBase::OnMaxStaminaChanged);
	// AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMovementSpeedAttribute()).AddUObject(this, &AMSCharacterBase::OnMovementSpeedChanged);

	// Apply default attributes and grant startup abilities.
	// This should ONLY happen on the server, and only once.
	if (HasAuthority())
	{
		ApplyStartupGAS();
	}
}

void AMSCharacterBase::ApplyStartupGAS()
{
	// This function ensures the startup process happens exactly once on the server.
	// It calls the virtual functions to grant attributes and abilities.
	if (HasAuthority() && AbilitySystemComponent && AbilitySystemComponent->GetAvatarActor() == this)
	{
		GrantStartingAttributes();
		GrantStartingAbilities();
	}
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


// Grant Default/Starting abilities to the character
// Default implementation does nothing. Derived classes will override.
void AMSCharacterBase::GrantStartingAbilities()
{
	// Default implementation does nothing. Derived classes MUST override this.
	UE_LOG(LogTemp, Error,
	       TEXT(
		       "AMSCharacterBase::GrantStartingAbilities - Base implementation called. Derived class MUST override this!"
	       ));
}

// --- Attribute Change Callbacks (to be overridden by derived classes) ---

// void AMSCharacterBase::OnHealthChanged(const FOnAttributeChangeData& Data)
// {
// 	// Default implementation does nothing. Derived classes can override to handle health changes.
// 	// This is where you would update UI or trigger effects based on health changes.
// }
