// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponManagerComponent.h"
#include "Characters/Base/MSCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Systems/WeaponSystem/Data/WeaponDataAsset.h"
#include "Systems/CombatSystem/CombatSystemComponent.h"
#include "Systems/WeaponSystem/WeaponBase.h"
#include "GameplayTags/MyNativeGameplayTags.h"
#include "Net/UnrealNetwork.h"

// ===================================
// Setup and overrides
// ===================================

// Sets default values for this component's properties
UWeaponManagerComponent::UWeaponManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}


// Called when the game starts
void UWeaponManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// This might change when we implement the combat component or to some interface
	// OwningCharacter = Cast<AMSCharacterBase>(GetOwner());
	
	// Set Combat System with Unarmed data
	if (UCombatSystemComponent* CombatComp = GetCombatSystemComponent())
	{
		CombatComp->UpdateCombatData(UnarmedData);
	}

	// Cache Weapon lookup in the beginning so it is fast to access later
	// BuildWeaponTagLookup();
}


// Called every frame
void UWeaponManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// Required override function
void UWeaponManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME is the macro that registers the variable for replication
	DOREPLIFETIME(UWeaponManagerComponent, EquippedWeaponInstance);

	// If you only want to replicate to the owner, you could use:
	// DOREPLIFETIME_CONDITION(UWeaponManagerComponent, EquippedWeaponInstance, COND_OwnerOnly);
}

AMSCharacterBase* UWeaponManagerComponent::GetOwningMSCharacter() const
{
	return Cast<AMSCharacterBase>(GetOwner());
}

UCombatSystemComponent* UWeaponManagerComponent::GetCombatSystemComponent() const
{
	return GetOwner()->FindComponentByClass<UCombatSystemComponent>();
}

// ===================================
// Actions: The actual functions to perform actions
// ===================================

// Equip weapon by tag and broadcast the event
// void UWeaponManagerComponent::EquipWeaponByTag(const FGameplayTag& WeaponTag)
// {
// 	if (!OwningCharacter)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::EquipWeaponByTag: OwningCharacter is invalid!"));
// 		return;
// 	}
//
// 	// Find weapon class by weapon tag from the available weapons
// 	TArray<TSubclassOf<AWeaponBase>> WeaponClasses;
// 	GetWeaponClassesForTag(WeaponTag, WeaponClasses);
//
// 	// Validate we found at least one class
// 	if (WeaponClasses.Num() == 0)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("UWeaponManagerComponent::EquipWeaponByTag: No weapon class found for tag %s"),
// 		       *WeaponTag.ToString());
// 		return;
// 	}
//
// 	// Get the first class since we expect only one class per tag
// 	// NOTE: In the future this might change to support multiple classes per tag
// 	TSubclassOf<AWeaponBase> WeaponClass = WeaponClasses[0];
//
// 	// if trying to equip the same weapon, unequip it and end the action
// 	if (EquippedWeaponInstance && EquippedWeaponInstance->IsA(WeaponClass))
// 	{
// 		UnequipWeapon(false);
// 		return;
// 	}
//
// 	// If a weapon is already equipped, unequip it first
// 	UnequipWeapon(true);
//
// 	SpawnWeaponForCharacter(WeaponClass);
// 	SetEquippedWeaponProperties();
//
// 	// Broadcast the delegate, passing the CLASS as defined in your signature
// 	OnWeaponEquipped.Broadcast(WeaponClass);
// }

// Equip weapon by the following weapon tag
void UWeaponManagerComponent::EquipWeaponByFollowingTag(const FGameplayTag& FollowingWeaponTag)
{
	// Only call this in the server
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}
 
	AMSCharacterBase* OwningCharacter = GetOwningMSCharacter();
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::EquipWeaponByFollowingTag: OwningCharacter is invalid!"));
		return;
	}

	TObjectPtr<UWeaponDataAsset> NewWeaponData;

	// Get the weapon to equip
	if (EquippedWeaponInstance)
	{
		// If the current equip weapon is the last index, equip the first weapon, else equip the next one
		if (FollowingWeaponTag.MatchesTagExact(TAG_Event_Weapon_Equip_Next))
		{
			int32 LastIndex = AvailableWeaponsData.Num() - 1;
			int32 EquippedWeaponDataInstanceIndex = AvailableWeaponsData.Find(EquippedWeaponInstance->WeaponData);

			if (EquippedWeaponDataInstanceIndex == INDEX_NONE)
			{
				UE_LOG(LogTemp, Error,
				       TEXT(
					       "UWeaponManagerComponent::EquipWeaponByFollowingTag: %s weapon was not found in the available weapons."
				       ), *EquippedWeaponInstance.GetName());
				return;
			}

			if (EquippedWeaponDataInstanceIndex == LastIndex)
			{
				NewWeaponData = AvailableWeaponsData[0];
			}
			else
			{
				NewWeaponData = AvailableWeaponsData[EquippedWeaponDataInstanceIndex + 1];
			}
		}
		// If the current equip weapon is the first index, equip the last weapon, else equip the previous one
		else if (FollowingWeaponTag.MatchesTagExact(TAG_Event_Weapon_Equip_Previous))
		{
			int32 EquippedWeaponDataInstanceIndex = AvailableWeaponsData.Find(EquippedWeaponInstance->WeaponData);

			if (EquippedWeaponDataInstanceIndex == INDEX_NONE)
			{
				UE_LOG(LogTemp, Error,
				       TEXT(
					       "UWeaponManagerComponent::EquipWeaponByFollowingTag: %s weapon was not found in the available weapons."
				       ), *EquippedWeaponInstance.GetName());
				return;
			}

			if (EquippedWeaponDataInstanceIndex == 0)
			{
				NewWeaponData = AvailableWeaponsData[AvailableWeaponsData.Num() - 1];
			}
			else
			{
				NewWeaponData = AvailableWeaponsData[EquippedWeaponDataInstanceIndex - 1];
			}
		}
	}
	else
	{
		if (FollowingWeaponTag.MatchesTagExact(TAG_Event_Weapon_Equip_Next))
		{
			NewWeaponData = AvailableWeaponsData[0];
		}
		else if (FollowingWeaponTag.MatchesTagExact(TAG_Event_Weapon_Equip_Previous))
		{
			NewWeaponData = AvailableWeaponsData[AvailableWeaponsData.Num() - 1];
		}
	}

	// If a weapon is already equipped, remove abilities
	if (EquippedWeaponInstance)
	{
		if (!EquippedWeaponInstance->WeaponData->AbilitiesToGrant.IsEmpty())
		{
			// Remove abilities of the weapon
			OwningCharacter->RemoveAbilities(AbilitiesGrantedByWeapon);
			AbilitiesGrantedByWeapon.Empty();
		}
		
	}

	// Equip the new weapon
	SpawnWeaponForCharacter(NewWeaponData);

	// Grant the abilities from the equipped weapon
	if (!EquippedWeaponInstance->WeaponData->AbilitiesToGrant.IsEmpty())
	{
		AbilitiesGrantedByWeapon = OwningCharacter->GrantAbilities(
			EquippedWeaponInstance->WeaponData->AbilitiesToGrant, EquippedWeaponInstance);
	}

	SetEquippedWeaponProperties(NewWeaponData);
	
	// Update Combat System with new weapon data
	if (UCombatSystemComponent* CombatComp = GetCombatSystemComponent())
	{
		CombatComp->UpdateCombatData(EquippedWeaponInstance->WeaponData);
	}

	// Broadcast the delegate
	OnWeaponEquipped.Broadcast(NewWeaponData);
}

// Unequip weapon and broadcast the event
void UWeaponManagerComponent::UnequipWeapon(bool bIsEquippingNextWeapon)
{
	// Only call this in the server
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (!EquippedWeaponInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("UWeaponManagerComponent::UnequipWeapon: No weapon is currently equipped."));
		return;
	}
	
	AMSCharacterBase* OwningCharacter = GetOwningMSCharacter();
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Log, TEXT("UWeaponManagerComponent::UnequipWeapon: OwningCharacter is invalid!"));
		return;
	}

	// Destroy equipped weapon instance
	EquippedWeaponInstance->Destroy();
	EquippedWeaponInstance = nullptr;

	// Remove abilities of the weapon
	OwningCharacter->RemoveAbilities(AbilitiesGrantedByWeapon);
	AbilitiesGrantedByWeapon.Empty();

	// Only set Unarmed properties and broadcast event if not equipping another weapon
	if (!bIsEquippingNextWeapon)
	{
		// Change back to Unarmed
		SetEquippedWeaponProperties(UnarmedData);

		// Update Combat System with Unarmed data
		if (UCombatSystemComponent* CombatComp = GetCombatSystemComponent())
		{
			CombatComp->UpdateCombatData(UnarmedData);
		}
		
		// Broadcast event
		OnWeaponUnequipped.Broadcast();
	}
}

void UWeaponManagerComponent::SpawnWeaponForCharacter(UWeaponDataAsset* InWeaponData)
{
	AMSCharacterBase* OwningCharacter = GetOwningMSCharacter();
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::SpawnWeaponForCharacter: OwningCharacter is invalid!"));
		return;
	}

	if (!InWeaponData)
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::SpawnWeaponForCharacter: WeaponData is invalid!"));
		return;
	}

	// Only spawn a new weapon actor if there is not already one
	if (!EquippedWeaponInstance)
	{
		// Define Spawning Parameters
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = OwningCharacter;
		SpawnParams.Instigator = OwningCharacter;

		// Spawning the Actor
		EquippedWeaponInstance = GetWorld()->SpawnActor<AWeaponBase>(
			AWeaponBase::StaticClass(),
			SpawnParams
		);
		
		if (!EquippedWeaponInstance)
		{
			UE_LOG(LogTemp, Error,
				   TEXT("UWeaponManagerComponent::SpawnWeaponForCharacter: Failed to spawn weapon of class %s"),
				   *InWeaponData->GetName());
			return;
		}
	}
	
	// For new weapon: Initialize the weapon with the weapon data
	// For switching weapon: Reinitialized with the new Weapon data
	EquippedWeaponInstance->Initialize(InWeaponData);
	EquippedWeaponInstance->AttachToCharacter(OwningCharacter->GetMesh());
}

// This funtion is called in the client to set the weapon properties like AnimClass and movement properties
void UWeaponManagerComponent::SetEquippedWeaponProperties(UWeaponDataAsset* InWeaponData)
{
	AMSCharacterBase* OwningCharacter = GetOwningMSCharacter();
	if (!OwningCharacter || !OwningCharacter->GetMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::SetWeaponAnimClass: OwningCharacter or OwningCharacter's mesh is invalid!"));
		return;
	}
	
	// Change the AnimInstance to the Weapon AnimInstance
	if (InWeaponData->WeaponAnimBP)
	{
		OwningCharacter->GetMesh()->SetAnimInstanceClass(InWeaponData->WeaponAnimBP);
	}

	// Set movement properties from the equipped weapon
	if (UCharacterMovementComponent* MovementComponent = OwningCharacter->GetCharacterMovement())
	{
		MovementComponent->MaxWalkSpeed = InWeaponData->MovementWeaponConfig.
		                                                                MaxWalkSpeed;
		MovementComponent->bOrientRotationToMovement = InWeaponData->MovementWeaponConfig.
		                                                                   OrientRotationToMovement;
		MovementComponent->bUseControllerDesiredRotation = InWeaponData->MovementWeaponConfig.
		                                                                       UseControllerDesiredRotation;
	}
}



// ===================================
// Replication functions
// ===================================


// This function only runs when the WeaponData is ready to be used
void UWeaponManagerComponent::OnWeaponDataReady(UWeaponDataAsset* InWeaponData)
{
	// Only Set Equipped Weapon Properties if the weapon instance is already fully replicated
	if (EquippedWeaponInstance && IsWeaponFullyReplicated(EquippedWeaponInstance))
	{
		SetEquippedWeaponProperties(InWeaponData);
	}
}

// This function only runs on clients after the variable changes.
void UWeaponManagerComponent::OnRep_EquippedWeaponInstance(AWeaponBase* OldWeapon)
{
	// Check if weapon is fully ready NOW
	if (IsWeaponFullyReplicated(EquippedWeaponInstance))
	{
		SetEquippedWeaponProperties(EquippedWeaponInstance ? EquippedWeaponInstance->WeaponData : UnarmedData);
	}
}

bool UWeaponManagerComponent::IsWeaponFullyReplicated(AWeaponBase* Weapon) const
{
	if (!Weapon)
	{
		return true; // Unequip case - no data needed
	}

	// Check that weapon instance exists AND has valid data
	if (!IsValid(Weapon->WeaponData))
	{
		return false;
	}

	// Optional: Add more checks if your weapon has other replicated properties
	// that must be ready (e.g., ammo count, attachments, etc.)

	return true;
}

// ===================================
// Weapon Tag Lookup Map
// ===================================

// Build the lookup map from tags to weapon classes
// void UWeaponManagerComponent::BuildWeaponTagLookup()
// {
// 	WeaponClassesByTag.Empty();
//
// 	for (const TSubclassOf<AWeaponBase>& WeaponClass : AvailableWeaponsData)
// 	{
// 		// Check valid class
// 		if (!*WeaponClass)
// 		{
// 			continue;
// 		}
// 		
// 		// Get class default object (CDO) to read WeaponData without spawning
// 		const AWeaponBase* CDO = WeaponClass->GetDefaultObject<AWeaponBase>();
// 		if (!CDO)
// 		{
// 			continue;
// 		}
// 		
// 		// Get tag from WeaponData
// 		const FGameplayTag& Tag = CDO->WeaponData->WeaponTag;
// 		if (Tag.IsValid())
// 		{
// 			WeaponClassesByTag.Add(Tag, WeaponClass);
// 		}
// 	}
// }

// Rebuild the lookup map from tags to weapon classes
// void UWeaponManagerComponent::RebuildWeaponTagLookup()
// {
// 	WeaponClassesByTag.Empty();
//
// 	for (const auto& WeaponClass : AvailableWeaponsData)
// 	{
// 		if (!*WeaponClass)
// 		{
// 			continue;
// 		}
//
// 		AWeaponBase* CDO = WeaponClass->GetDefaultObject<AWeaponBase>();
// 		if (CDO && CDO->WeaponData->WeaponTag.IsValid())
// 		{
// 			WeaponClassesByTag.Add(CDO->WeaponData->WeaponTag, WeaponClass);
// 		}
// 	}
// }

// Get weapon classes for a given tag
// void UWeaponManagerComponent::GetWeaponClassesForTag(const FGameplayTag& Tag,
//                                                      TArray<TSubclassOf<AWeaponBase>>& OutClasses) const
// {
// 	OutClasses.Reset();
// 	WeaponClassesByTag.MultiFind(Tag, OutClasses);
// }

// ===================================
// Debug Utils
// ===================================

// Use to print logs to debug weapon tag cache 
// void UWeaponManagerComponent::PrintWeaponTagCache() const
// {
// 	UE_LOG(LogTemp, Log, TEXT("WeaponClassesByTag cache contents:"));
//
// 	// First get all unique keys (tags)
// 	TArray<FGameplayTag> UniqueTags;
// 	WeaponClassesByTag.GenerateKeyArray(UniqueTags);
//
// 	// Iterate each unique tag
// 	for (const FGameplayTag& Tag : UniqueTags)
// 	{
// 		// Get all weapon classes for this tag
// 		TArray<TSubclassOf<AWeaponBase>> Classes;
// 		WeaponClassesByTag.MultiFind(Tag, Classes);
//
// 		FString ClassNames;
// 		for (const TSubclassOf<AWeaponBase>& WeaponClass : Classes)
// 		{
// 			if (*WeaponClass)
// 			{
// 				ClassNames += WeaponClass->GetName() + TEXT(", ");
// 			}
// 			else
// 			{
// 				ClassNames += TEXT("InvalidClass, ");
// 			}
// 		}
//
// 		UE_LOG(LogTemp, Log, TEXT("Tag: %s => Classes: %s"), *Tag.ToString(), *ClassNames);
// 	}
// }
