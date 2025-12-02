// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h" // The use of character can change to the combat component
#include "GameplayTagContainer.h"
#include "Data/FWeaponConfig.h"
#include "WeaponManagerComponent.generated.h"

class AWeaponBase;

//  Define the Delegate Signature (no parameters in this example)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquippedSignature, TSubclassOf<AWeaponBase>, WeaponClass);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponUnequippedSignature);

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

	// ===================================
	// Properties & Configurations (Editable Variables)
	// ===================================
public:
	// Array of available weapon classes to equip from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	TArray<TSubclassOf<AWeaponBase>> AvailableWeapons;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapons | Abilities")
	TArray<FGameplayAbilitySpecHandle> AbilitiesGrantedByWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unarmed Properties")
	FWeaponConfig UnarmedConfig;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "References")
	TObjectPtr<class AMSCharacterBase> OwningCharacter;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Runtime", ReplicatedUsing=OnRep_EquippedWeaponInstance)
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
	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void EquipWeaponByTag(const FGameplayTag& WeaponTag);

	// Equip weapon by following weapon tag, equips the "Next" or "Previous" weapon in the AvailableWeapons array
	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void EquipWeaponByFollowingTag(const FGameplayTag& WeaponTag);

	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void UnequipWeapon(bool bIsEquippingNextWeapon);

protected:
	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void SpawnWeaponForCharacter(TSubclassOf<AWeaponBase> WeaponClass);

	UFUNCTION(BlueprintCallable, Category = "Weapon Actions")
	void SetEquippedWeaponProperties();

	// ===================================
	// Replication functions
	// ===================================

	UFUNCTION()
	void OnRep_EquippedWeaponInstance(AWeaponBase* OldWeapon);


	// ===================================
	// Weapon Tag Lookup Map 
	// ===================================
public:
	// Lookup weapon classes by tag quickly
	void GetWeaponClassesForTag(const FGameplayTag& Tag, TArray<TSubclassOf<AWeaponBase>>& OutClasses) const;

protected:
	// Lookup cache from Tag to weapon classes
	TMultiMap<FGameplayTag, TSubclassOf<AWeaponBase>> WeaponClassesByTag;

	// Call this at begin play or when WeaponClasses array changes
	// NOTE: if the AvailableWeapons array changes at runtime, this needs to be called again when the AvailableWeapons array changes to rebuild the lookup
	void BuildWeaponTagLookup();

	// Rebuild the lookup map from tags to weapon classes
	// NOTE: useful if the AvailableWeapons array changes at runtime
	void RebuildWeaponTagLookup();

	// ===================================
	// Debug Utils
	// ===================================
private:
	// Use to print logs to debug weapon tag cache
	void PrintWeaponTagCache() const;
};
