// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "AbilitySystemInterface.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();

	// Check for null early
	if (!OwnerActor)
	{
		return;
	}

	// Safely cast the Owner to the IAbilitySystemInterface
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OwnerActor);

	// Check if the owner implements the interface
	if (!ASCInterface)
	{
		// The owner doesn't implement the standard interface for GAS. Log a warning.
		UE_LOG(LogTemp, Warning, TEXT("Component owner does not implement IAbilitySystemInterface."));
		return;
	}

	// Get the base UAbilitySystemComponent*
	UAbilitySystemComponent* BaseASC = ASCInterface->GetAbilitySystemComponent();

	// Check if the component was successfully returned
	if (!BaseASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("IAbilitySystemInterface returned a null AbilitySystemComponent."));
		return;
	}


	// Cache the reference after all safe checks
	CachedASC = Cast<UMSAbilitySystemComponent>(BaseASC);
	CachedAttributeSet = const_cast<UMSAttributeSet*>(CachedASC->GetSet<UMSAttributeSet>());
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
