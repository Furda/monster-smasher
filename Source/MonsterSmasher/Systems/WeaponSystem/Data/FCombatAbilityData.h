#pragma once


#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FCombatAbilityData.generated.h"

struct FGameplayTag;
class UGameplayEffect;

/**
 *  Describes how a specific ability behaves with this weapon (animations, damage values, combo chains).
 */
USTRUCT(BlueprintType)
struct FCombatAbilityData
{
	GENERATED_BODY()

	// Combo chain animations (index = combo step)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	TArray<UAnimMontage*> AbilityMontages;

	// Damage per combo step (optional, can scale damage)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	// Window after each attack where next combo input is valid
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	float ComboInputWindowDuration  = 0.5f;

	// Valid next combo attacks (for combo tree validation)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Combo")
	TArray<FGameplayTag> ValidNextComboTags;

	// Optional: Per-attack movement modifiers
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TSubclassOf<UGameplayEffect> MovementModifierEffect;
};
