// Fill out your copyright notice in the Description page of Project Settings.


#include "MSPlayerState.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"


AMSPlayerState::AMSPlayerState()
{
	// Ensure PlayerState replicates to all clients for multiplayer setup
	// High update rate for fast attribute changes
	SetNetUpdateFrequency(100.f);

	// Create AbilitySystemComponent and set it to be replicated
	AbilitySystemComponent = CreateDefaultSubobject<UMSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the AttributeSet and register it
	AttributeSet = CreateDefaultSubobject<UMSAttributeSet>(TEXT("AttributeSet"));

	// Prevent PlayerState from ticking unless explicitly enabled.
	// GAS handles its own internal ticking.
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* AMSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMSAttributeSet* AMSPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

void AMSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// Initialize GAS
	InitAbilitySystemAndAttributes();
}

void AMSPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// You can add any per-tick logic here if needed.
	// For GAS, most logic is handled by the AbilitySystemComponent.
}

void AMSPlayerState::InitAbilitySystemAndAttributes()
{
	// This function can be called multiple times due to replication,
	// so ensure InitAbilityActorInfo is only called once per valid Pawn.
	// If the AvatarActor changes, reinitialize.

	APawn* CharacterPawn = GetPawn(); // Get the currently possessed pawn

	// Only proceed if ASC is valid, and we have a Pawn, and it's a *new* Pawn or first initialization
	if (AbilitySystemComponent && CharacterPawn && CharacterPawn != CachedAvatarActor)
	{
		// InitAbilityActorInfo sets up the internal GAS pointers (OwnerActor, AvatarActor, PlayerController)
		AbilitySystemComponent->InitAbilityActorInfo(this, CharacterPawn); // PlayerState is Owner, Character is Avata

		CachedAvatarActor = CharacterPawn; // Cache the current avatar to prevent redundant re-initialization
		UE_LOG(LogTemp, Log,
		       TEXT("AMSPlayerState::InitAbilitySystemAndAttributes: GAS Initialized. Owner: %s, Avatar: %s"),
		       *GetNameSafe(this), *GetNameSafe(CharacterPawn));
	}
	else if (!AbilitySystemComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("AMSPlayerState::InitAbilitySystemAndAttributes: AbilitySystemComponent is NULL."));
	}
	else if (!CharacterPawn)
	{
		UE_LOG(LogTemp, Log,
		       TEXT(
			       "AMSPlayerState::InitAbilitySystemAndAttributes: Character Pawn is NULL. Will re-attempt when possessed."
		       ));
	}
}
