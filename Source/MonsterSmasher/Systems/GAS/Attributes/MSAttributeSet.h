// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MSAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class MONSTERSMASHER_API UMSAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMSAttributeSet();

	// Override PreAttributeChange to clamp attributes before they are changed
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	// Override PostGameplayEffectExecute to handle attribute changes after a GE is applied
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	// -------------------------------------------------------------------
	// Primary Attributes
	// -------------------------------------------------------------------
	
	// ----- Health Attributes -----
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,ReplicatedUsing=OnRep_Health, Category = "Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMSAttributeSet, Health)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_MaxHealth, Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMSAttributeSet, MaxHealth)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData MetaHealth;
	ATTRIBUTE_ACCESSORS(UMSAttributeSet, MetaHealth)

	// Replicate changes to Health
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	

	// ----- Stamina Attributes -----
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Stamina, Category = "Attributes")
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UMSAttributeSet, Stamina)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_MaxStamina, Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UMSAttributeSet, MaxStamina)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	FGameplayAttributeData MetaStamina;
	ATTRIBUTE_ACCESSORS(UMSAttributeSet, MetaStamina)

	// Replicate changes to Stamina
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldStamina) const;

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina) const;

	
	// -------------------------------------------------------------------
	// Replication Overrides
	// -------------------------------------------------------------------

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
