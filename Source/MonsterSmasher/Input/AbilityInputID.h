#pragma once

#include "CoreMinimal.h"
#include "AbilityInputID.generated.h"

/**
 * Enumerates the default input IDs for gameplay abilities in MonsterSmasher.
 * These are used to bind input actions to specific gameplay abilities.
 *
 * It's good practice to have a "None" and potentially "Confirm/Cancel"
 * as universal inputs, then specific ability slots.
 */
UENUM(BlueprintType)
enum class EAbilityInputID : uint8
{
	// 0: No input bound
	None = 0,

	// 1: Generic Confirm input (e.g., Primary Attack, Interact)
	Confirm,

	// 2: Generic Cancel input (e.g., Secondary Attack, Dodge, Block)
	Cancel,

	// 3: Jump
	Jump,

	// 4: Sprint
	Sprint,

	// 5: Dodge
	Dodge,

	// 6: Block
	Block,

	// 7: Light Attack
	LightAttack,

	// 8: HeavyAttack
	HeavyAttack,

	// 9: Specific Ability Slot 1 (e.g., Q ability)
	Ability1,

	// 10: Specific Ability Slot 2 (e.g., E ability)
	Ability2,

	// 11: Specific Ability Slot 3 (e.g., Shift ability)
	Ability3,

	// 12: Specific Ability Slot 4 (e.g., R ability)
	Ability4,

	// Any other specific input IDs you might need

	Max // Keep this as the last entry, useful for iteration or array sizing
};
