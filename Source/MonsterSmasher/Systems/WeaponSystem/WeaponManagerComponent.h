// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "WeaponManagerComponent.generated.h"

class UCombatSystemComponent;
class AMSCharacterBase;
class UWeaponDataAsset;
class AWeaponBase;


//  Define the Delegate Signature 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquippedSignature, UWeaponDataAsset*, WeaponData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUnequippedSignature);

/**
 * @class UWeaponManagerComponent
 * @brief Responsible for managing weapons and weapon-related functionalities for a character.
 *
 * The UWeaponManagerComponent class allows a character to equip, unequip, and manage weapons dynamically.
 * Provides functionality to associate weapon data, assign abilities, and handle weapon cycling based on gameplay tags.
 * This component also manages replication of weapon states for multiplayer.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MONSTERSMASHER_API UWeaponManagerComponent : public UActorComponent
{
	GENERATED_BODY()

	// ===================================
	// Setup and overrides
	// ===================================
public:
	// Sets default values for this component's properties
	UWeaponManagerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	AMSCharacterBase* GetOwningMSCharacter() const;
	
	UCombatSystemComponent* GetCombatSystemComponent() const;

	// ===================================
	// Properties & Configurations
	// ===================================
public:
	// Array of available weapon classes to equip from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<TObjectPtr<UWeaponDataAsset>> AvailableWeaponsData;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unarmed Data")
	TObjectPtr<UWeaponDataAsset> UnarmedData;
	
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Weapons | Abilities")
	TArray<FGameplayAbilitySpecHandle> AbilitiesGrantedByWeapon;

	// UPROPERTY(BlueprintReadOnly, Transient, Category = "References")
	// TObjectPtr<AMSCharacterBase> OwningCharacter;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Weapon", ReplicatedUsing=OnRep_EquippedWeaponInstance)
	TObjectPtr<AWeaponBase> EquippedWeaponInstance;

	// ===================================
	// Events: Delegate Declarations
	// ===================================

public:
	UPROPERTY(BlueprintAssignable, Category = "Weapon Events")
	FOnWeaponEquippedSignature OnWeaponEquipped;

	UPROPERTY(BlueprintAssignable, Category = "Weapon Events")
	FOnWeaponUnequippedSignature OnWeaponUnequipped;

	// ===================================
	// Actions: The actual functions to perform actions
	// ===================================

	// Equip weapon by weapon tag
	// UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	// void EquipWeaponByTag(const FGameplayTag& WeaponTag);

	// Equip weapon by following weapon tag, equips the "Next" or "Previous" weapon in the AvailableWeapons array
	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void EquipWeaponByFollowingTag(const FGameplayTag& WeaponTag);

	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void UnequipWeapon(bool bIsEquippingNextWeapon);

protected:
	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void SpawnWeaponForCharacter(UWeaponDataAsset* InWeaponData);

	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void SetEquippedWeaponProperties(UWeaponDataAsset* InWeaponData);
	

	// ===================================
	// Replication functions
	// ===================================

public:
	
	// Called by weapon instance when its data is ready
	UFUNCTION()
	void OnWeaponDataReady(UWeaponDataAsset* InWeaponData);

protected:
	
	UFUNCTION()
	void OnRep_EquippedWeaponInstance(AWeaponBase* OldWeapon);
	
	
	bool bIsSwitchingWeapons = false;
    
	// Validation helper
	bool IsWeaponFullyReplicated(AWeaponBase* Weapon) const;



	// ===================================
	// Weapon Tag Lookup Map 
	// ===================================
	
public:
	// // Lookup weapon classes by tag quickly
	// void GetWeaponClassesForTag(const FGameplayTag& Tag, TArray<TSubclassOf<AWeaponBase>>& OutClasses) const;

protected:
	// Lookup cache from Tag to weapon classes
	// TMultiMap<FGameplayTag, TSubclassOf<AWeaponBase>> WeaponClassesByTag;

	// Call this at begin play or when WeaponClasses array changes
	// NOTE: if the AvailableWeapons array changes at runtime, this needs to be called again when the AvailableWeapons array changes to rebuild the lookup
	// void BuildWeaponTagLookup();

	// Rebuild the lookup map from tags to weapon classes
	// NOTE: useful if the AvailableWeapons array changes at runtime
	// void RebuildWeaponTagLookup();

	// ===================================
	// Debug Utils
	// ===================================
private:
	// Use to print logs to debug weapon tag cache
	// void PrintWeaponTagCache() const;
};
