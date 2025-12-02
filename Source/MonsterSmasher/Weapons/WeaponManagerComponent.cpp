// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponManagerComponent.h"
#include "Characters/Base/MSCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayTags/MyNativeGameplayTags.h"
#include "Net/UnrealNetwork.h" // Required for replication macros
#include "Weapons/WeaponBase.h"

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
	OwningCharacter = Cast<AMSCharacterBase>(GetOwner());

	// Cache Weapon lookup in the beginning so it is fast to access later
	BuildWeaponTagLookup();
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

// ===================================
// Actions: The actual functions to perform actions
// ===================================

// Equip weapon by tag and broadcast the event
void UWeaponManagerComponent::EquipWeaponByTag(const FGameplayTag& WeaponTag)
{
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::EquipWeaponByTag: OwningCharacter is invalid!"));
		return;
	}

	// Find weapon class by weapon tag from the available weapons
	TArray<TSubclassOf<AWeaponBase>> WeaponClasses;
	GetWeaponClassesForTag(WeaponTag, WeaponClasses);

	// Validate we found at least one class
	if (WeaponClasses.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UWeaponManagerComponent::EquipWeaponByTag: No weapon class found for tag %s"),
		       *WeaponTag.ToString());
		return;
	}

	// Get the first class since we expect only one class per tag
	// NOTE: In the future this might change to support multiple classes per tag
	TSubclassOf<AWeaponBase> WeaponClass = WeaponClasses[0];

	// if trying to equip the same weapon, unequip it and end the action
	if (EquippedWeaponInstance && EquippedWeaponInstance->IsA(WeaponClass))
	{
		UnequipWeapon(false);
		return;
	}

	// If a weapon is already equipped, unequip it first
	UnequipWeapon(true);

	SpawnWeaponForCharacter(WeaponClass);
	SetEquippedWeaponProperties();

	// Broadcast the delegate, passing the CLASS as defined in your signature
	OnWeaponEquipped.Broadcast(WeaponClass);
}

// Equip weapon by the following weapon tag
void UWeaponManagerComponent::EquipWeaponByFollowingTag(const FGameplayTag& FollowingWeaponTag)
{
	// Only call this in the server
	if (GetOwnerRole() != ROLE_Authority)
	{
		return;
	}

	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::EquipWeaponByFollowingTag: OwningCharacter is invalid!"));
		return;
	}

	TSubclassOf<AWeaponBase> WeaponClass;

	// Get the weapon to equip
	if (EquippedWeaponInstance)
	{
		// If the current equip weapon is the last index, equip the first weapon, else equip the next one
		if (FollowingWeaponTag.MatchesTagExact(TAG_Event_Weapon_Equip_Next))
		{
			int32 LastIndex = AvailableWeapons.Num() - 1;
			int32 EquippedWeaponInstanceIndex = AvailableWeapons.Find(EquippedWeaponInstance.GetClass());

			if (EquippedWeaponInstanceIndex == INDEX_NONE)
			{
				UE_LOG(LogTemp, Error,
				       TEXT(
					       "UWeaponManagerComponent::EquipWeaponByFollowingTag: %s weapon was not found in the available weapons."
				       ), *EquippedWeaponInstance.GetName());
				return;
			}

			if (EquippedWeaponInstanceIndex == LastIndex)
			{
				WeaponClass = AvailableWeapons[0];
			}
			else
			{
				WeaponClass = AvailableWeapons[EquippedWeaponInstanceIndex + 1];
			}
		}
		// If the current equip weapon is the first index, equip the last weapon, else equip the previous one
		else if (FollowingWeaponTag.MatchesTagExact(TAG_Event_Weapon_Equip_Previous))
		{
			int32 EquippedWeaponInstanceIndex = AvailableWeapons.Find(EquippedWeaponInstance.GetClass());

			if (EquippedWeaponInstanceIndex == INDEX_NONE)
			{
				UE_LOG(LogTemp, Error,
				       TEXT(
					       "UWeaponManagerComponent::EquipWeaponByFollowingTag: %s weapon was not found in the available weapons."
				       ), *EquippedWeaponInstance.GetName());
				return;
			}

			if (EquippedWeaponInstanceIndex == 0)
			{
				WeaponClass = AvailableWeapons[AvailableWeapons.Num() - 1];
			}
			else
			{
				WeaponClass = AvailableWeapons[EquippedWeaponInstanceIndex - 1];
			}
		}
	}
	else
	{
		if (FollowingWeaponTag.MatchesTagExact(TAG_Event_Weapon_Equip_Next))
		{
			WeaponClass = AvailableWeapons[0];
		}
		else if (FollowingWeaponTag.MatchesTagExact(TAG_Event_Weapon_Equip_Previous))
		{
			WeaponClass = AvailableWeapons[AvailableWeapons.Num() - 1];
		}
	}

	// If a weapon is already equipped, unequip it first
	if (EquippedWeaponInstance)
	{
		UnequipWeapon(true);
	}

	// Equip the new weapon
	SpawnWeaponForCharacter(WeaponClass);

	// Grant the abilities from the equipped weapon
	if (!EquippedWeaponInstance->WeaponConfig.AbilitiesToGrant.IsEmpty())
	{
		AbilitiesGrantedByWeapon = OwningCharacter->GrantAbilities(
			EquippedWeaponInstance->WeaponConfig.AbilitiesToGrant);
	}

	SetEquippedWeaponProperties();

	// Broadcast the delegate
	OnWeaponEquipped.Broadcast(WeaponClass);
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
		SetEquippedWeaponProperties();

		// Broadcast event
		OnWeaponUnequipped.Broadcast();
	}
}

void UWeaponManagerComponent::SpawnWeaponForCharacter(TSubclassOf<AWeaponBase> WeaponClass)
{
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::SpawnWeaponForCharacter: OwningCharacter is invalid!"));
		return;
	}

	if (!WeaponClass)
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::SpawnWeaponForCharacter: WeaponClass is invalid!"));
		return;
	}

	// Define Spawning Parameters
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwningCharacter;
	SpawnParams.Instigator = OwningCharacter;

	// Spawning the Actor
	const FVector Location = FVector::ZeroVector;
	const FRotator Rotation = FRotator::ZeroRotator;

	EquippedWeaponInstance = GetWorld()->SpawnActor<AWeaponBase>(
		WeaponClass,
		Location,
		Rotation,
		SpawnParams
	);

	if (!EquippedWeaponInstance)
	{
		UE_LOG(LogTemp, Error,
		       TEXT("UWeaponManagerComponent::SpawnWeaponForCharacter: Failed to spawn weapon of class %s"),
		       *WeaponClass->GetName());
		return;
	}

	// Attaching the Weapon Actor to the Socket
	// We use an FAttachmentTransformRules for safety and clarity
	const FAttachmentTransformRules AttachRules(EAttachmentRule::SnapToTarget, true);

	EquippedWeaponInstance->AttachToComponent(
		OwningCharacter->GetMesh(),
		AttachRules,
		EquippedWeaponInstance->WeaponConfig.EquippedSocketName
	);
}

// This funtion is called in the client to set the weapon properties like AnimClass and movement properties
void UWeaponManagerComponent::SetEquippedWeaponProperties()
{
	if (!OwningCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UWeaponManagerComponent::SetWeaponAnimClass: OwningCharacter is invalid!"));
		return;
	}

	FWeaponConfig CurrentWeaponConfig = EquippedWeaponInstance ? EquippedWeaponInstance->WeaponConfig : UnarmedConfig;

	// Change the AnimInstance to the Weapon AnimInstance
	OwningCharacter->GetMesh()->SetAnimInstanceClass(CurrentWeaponConfig.AnimClass);

	// Set movement properties from the equipped weapon
	if (UCharacterMovementComponent* MovementComponent = OwningCharacter->GetCharacterMovement())
	{
		MovementComponent->MaxCustomMovementSpeed = CurrentWeaponConfig.MovementWeaponConfig.
		                                                                MaxWalkSpeed;
		MovementComponent->bOrientRotationToMovement = CurrentWeaponConfig.MovementWeaponConfig.
		                                                                   OrientRotationToMovement;
		MovementComponent->bUseControllerDesiredRotation = CurrentWeaponConfig.MovementWeaponConfig.
		                                                                       UseControllerDesiredRotation;
	}
}

// ===================================
// Replication functions
// ===================================

// This function only runs on clients after the variable changes.
void UWeaponManagerComponent::OnRep_EquippedWeaponInstance(AWeaponBase* OldWeapon)
{
	// Apply the cosmetic change based on the new weapon
	SetEquippedWeaponProperties();
}

// ===================================
// Weapon Tag Lookup Map
// ===================================

// Build the lookup map from tags to weapon classes
void UWeaponManagerComponent::BuildWeaponTagLookup()
{
	WeaponClassesByTag.Empty();

	for (const TSubclassOf<AWeaponBase>& WeaponClass : AvailableWeapons)
	{
		// Check valid class
		if (!*WeaponClass)
		{
			continue;
		}

		// Get class default object (CDO) to read WeaponConfig without spawning
		const AWeaponBase* CDO = WeaponClass->GetDefaultObject<AWeaponBase>();
		if (!CDO)
		{
			continue;
		}

		// Get tag from WeaponConfig
		const FGameplayTag& Tag = CDO->WeaponConfig.WeaponTag;
		if (Tag.IsValid())
		{
			WeaponClassesByTag.Add(Tag, WeaponClass);
		}
	}
}

// Rebuild the lookup map from tags to weapon classes
void UWeaponManagerComponent::RebuildWeaponTagLookup()
{
	WeaponClassesByTag.Empty();

	for (const auto& WeaponClass : AvailableWeapons)
	{
		if (!*WeaponClass)
		{
			continue;
		}

		AWeaponBase* CDO = WeaponClass->GetDefaultObject<AWeaponBase>();
		if (CDO && CDO->WeaponConfig.WeaponTag.IsValid())
		{
			WeaponClassesByTag.Add(CDO->WeaponConfig.WeaponTag, WeaponClass);
		}
	}
}

// Get weapon classes for a given tag
void UWeaponManagerComponent::GetWeaponClassesForTag(const FGameplayTag& Tag,
                                                     TArray<TSubclassOf<AWeaponBase>>& OutClasses) const
{
	OutClasses.Reset();
	WeaponClassesByTag.MultiFind(Tag, OutClasses);
}

// ===================================
// Debug Utils
// ===================================

// Use to print logs to debug weapon tag cache 
void UWeaponManagerComponent::PrintWeaponTagCache() const
{
	UE_LOG(LogTemp, Log, TEXT("WeaponClassesByTag cache contents:"));

	// First get all unique keys (tags)
	TArray<FGameplayTag> UniqueTags;
	WeaponClassesByTag.GenerateKeyArray(UniqueTags);

	// Iterate each unique tag
	for (const FGameplayTag& Tag : UniqueTags)
	{
		// Get all weapon classes for this tag
		TArray<TSubclassOf<AWeaponBase>> Classes;
		WeaponClassesByTag.MultiFind(Tag, Classes);

		FString ClassNames;
		for (const TSubclassOf<AWeaponBase>& WeaponClass : Classes)
		{
			if (*WeaponClass)
			{
				ClassNames += WeaponClass->GetName() + TEXT(", ");
			}
			else
			{
				ClassNames += TEXT("InvalidClass, ");
			}
		}

		UE_LOG(LogTemp, Log, TEXT("Tag: %s => Classes: %s"), *Tag.ToString(), *ClassNames);
	}
}
