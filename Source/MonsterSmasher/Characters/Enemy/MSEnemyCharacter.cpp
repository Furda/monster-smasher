// Fill out your copyright notice in the Description page of Project Settings.


#include "MSEnemyCharacter.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"


// Sets default values
AMSEnemyCharacter::AMSEnemyCharacter()
{
	// Control by AI
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Create and set Ability System Component
	AbilitySystemComponent = CreateDefaultSubobject<UMSAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// Create the AttributeSet.
	AttributeSet = CreateDefaultSubobject<UMSAttributeSet>(TEXT("AttributeSet"));
}

void AMSEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitAbilitySystemAndAttributes();
}

UAbilitySystemComponent* AMSEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMSAttributeSet* AMSEnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void AMSEnemyCharacter::GrantStartingAbilities()
{
	UE_LOG(LogTemp, Log, TEXT("AMSEnemyCharacter::GrantStartingAbilities - Granting abilities."));
}
