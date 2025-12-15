// Fill out your copyright notice in the Description page of Project Settings.



#include "CombatSystemComponent.h"

#include "AbilitySystemInterface.h"
#include "Systems/GAS/Attributes/MSAttributeSet.h"
#include "Systems/GAS/AbilitySystem/MSAbilitySystemComponent.h"
#include "Systems/WeaponSystem/Data/WeaponDataAsset.h"



// =================================
// Set up and overrides
// =================================

// Sets default values for this component's properties
UCombatSystemComponent::UCombatSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatSystemComponent::BeginPlay()
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
void UCombatSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


// ====================================================
// Weapon data management
// ====================================================

void UCombatSystemComponent::UpdateCombatData(UWeaponDataAsset* NewWeaponData)
{
	if (!NewWeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCombatSystemComponent::UpdateCombatData - NewWeaponData is null"));
		CurrentWeaponData = nullptr;
		CachedAbilityData.Empty();
		return;
	}
	
	CurrentWeaponData = NewWeaponData;
    
	// TODO: double check which tags to use for the combat component
	// Save in UPROPERTY variables so they can be configure in the editor? 
	// OR Hardcode tags like TAG_InputTag_Attack_Light
	
	// Cache ability data by tag for fast lookup
	CachedAbilityData.Empty();
	CachedAbilityData.Add(LightAttackTag, NewWeaponData->LightAttackData);
	CachedAbilityData.Add(HeavyAttackTag, NewWeaponData->HeavyAttackData);
	CachedAbilityData.Add(BlockTag, NewWeaponData->BlockData);
    
	// Broadcast event so active abilities can refresh
	OnCombatDataUpdated.Broadcast();
}

const FCombatAbilityData* UCombatSystemComponent::GetAbilityData(FGameplayTag AbilityTag) const
{
	// Look up ability data in the cached map
	const FCombatAbilityData* FoundData = CachedAbilityData.Find(AbilityTag);
    
	if (!FoundData)
	{
		UE_LOG(LogTemp, Warning, TEXT("UCombatSystemComponent::GetAbilityData - No data found for tag: %s"), 
			*AbilityTag.ToString());
		return nullptr;
	}

	return FoundData;
}

// Blueprint-safe version
bool UCombatSystemComponent::GetAbilityData(FGameplayTag AbilityTag, FCombatAbilityData& OutAbilityData) const
{
	if (const FCombatAbilityData* FoundData = CachedAbilityData.Find(AbilityTag))
	{
		OutAbilityData = *FoundData;
		return true;
	}
    
	return false;
}


// ====================================================
// Validation
// ====================================================

bool UCombatSystemComponent::CanActivateAbility(FGameplayTag AbilityTag, FGameplayTagContainer& OutFailureTags) const
{
	
	
	// Combo validation
	if (AbilityTag.MatchesTag(FGameplayTag::RequestGameplayTag("Combat.Ability.LightAttack")))
	{
		// Logic to determine if the Light attack can be activated
		
		// float TimeSinceLastAttack = GetWorld()->GetTimeSeconds() - LastAttackTime;
		// if (CurrentComboIndex > 0 && TimeSinceLastAttack > ComboWindowDuration)
		// {
		// 	OutFailureTags.AddTag(FGameplayTag::RequestGameplayTag("Combat.Failure.ComboWindowExpired"));
		// 	return false;
		// }
		//
		// if (CurrentComboIndex >= MaxComboLength)
		// {
		// 	OutFailureTags.AddTag(FGameplayTag::RequestGameplayTag("Combat.Failure.ComboLimitReached"));
		// 	return false;
		// }
	}

	return true;
}


// ====================================================
// Combo System Management
// ====================================================

void UCombatSystemComponent::RegisterComboAttack(FGameplayTag AttackTag)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Check if this is a new combo chain or continuation
	if (ActiveComboTag != AttackTag || !IsInComboWindow())
	{
		// Start new combo
		CurrentComboIndex = 0;
		ActiveComboTag = AttackTag;
	}
	else
	{
		// Progress combo
		CurrentComboIndex++;
	}

	LastComboInputTime = CurrentTime;

	// Get weapon data to determine window duration
	if (const FCombatAbilityData* AbilityData = GetAbilityData(AttackTag))
	{
		// Set timer to reset combo if no input received
		GetWorld()->GetTimerManager().SetTimer(
			ComboResetTimerHandle,
			this,
			&UCombatSystemComponent::OnComboWindowExpired,
			AbilityData->ComboInputWindowDuration,
			false
		);
	}
}

UAnimMontage* UCombatSystemComponent::GetCurrentComboMontage(FGameplayTag AttackTag) const
{
	const FCombatAbilityData* AbilityData = GetAbilityData(AttackTag);
	if (AbilityData->AbilityMontages.Num() == 0)
		return nullptr;

	// Clamp to available montages
	int32 SafeIndex = FMath::Min(CurrentComboIndex, AbilityData->AbilityMontages.Num() - 1);
	return AbilityData->AbilityMontages[SafeIndex];
}

bool UCombatSystemComponent::IsInComboWindow() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(ComboResetTimerHandle);
}

void UCombatSystemComponent::ResetCombo()
{
	CurrentComboIndex = 0;
	ActiveComboTag = FGameplayTag::EmptyTag;
	GetWorld()->GetTimerManager().ClearTimer(ComboResetTimerHandle);
}

void UCombatSystemComponent::OnComboWindowExpired()
{
	UE_LOG(LogTemp, Log, 
		TEXT("UCombatSystemComponent::OnComboWindowExpired - Combo chain reset for tag: %s"), 
		*ActiveComboTag.ToString());
    
	// Reset combo state
	ResetCombo();
}