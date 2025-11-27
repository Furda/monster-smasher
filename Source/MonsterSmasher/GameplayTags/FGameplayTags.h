#pragma once

#include "CoreMinimal.h"
#include "GameplayTagsManager.h"
#include "FGameplayTags.generated.h"

// struct FGameplayTagDetails
// {
// 	static FGameplayTag Tag;
// 	static FString Description;
// };


/**
 * FGameplayTags: Centralized definition and initialization of gameplay tags used throughout the game.
 */
USTRUCT(BlueprintType)
struct FGameplayTags
{
	GENERATED_BODY()

	// ----- InputTags -----

	static FGameplayTag InputTag_Jump;
	static FGameplayTag InputTag_Dodge;
	static FGameplayTag InputTag_Sprint;
	static FGameplayTag InputTag_Interact;
	static FGameplayTag InputTag_Attack_Light;
	static FGameplayTag InputTag_Attack_Heavy;
	static FGameplayTag InputTag_Block;
	static FGameplayTag InputTag_TargetLock;
	static FGameplayTag InputTag_EquipWeapon;

	// Ability input tags
	static FGameplayTag InputTag_Ability1;

	// ----- Abilities -----

	// General Attack Abilities
	static FGameplayTag Ability_Attack_Light;
	static FGameplayTag Ability_Attack_Heavy;
	static FGameplayTag Ability_Attack_Ranged;
	static FGameplayTag Ability_Attack_Special;

	// Defense Abilities
	static FGameplayTag Ability_Defense_Block;


	// Utility Abilities

	static FGameplayTag Ability_Utility_EquipWeapon;
	static FGameplayTag Ability_Utility_Heal;
	static FGameplayTag Ability_Utility_Buff;
	static FGameplayTag Ability_Utility_Interact;

	// Movement Abilities
	static FGameplayTag Ability_Movement_Sprint;
	static FGameplayTag Ability_Movement_Dodge;

	// ----- Weapons -----

	// Melee Weapons
	static FGameplayTag Weapon_Type_Sword;
	static FGameplayTag Weapon_Type_Axe;
	static FGameplayTag Weapon_Type_Fist;

	// Range Weapons
	static FGameplayTag Weapon_Type_Bow;
	static FGameplayTag Weapon_Type_MagicStaff;

	// ----- States -----

	static FGameplayTag State_Dead;
	static FGameplayTag State_Stunned;
	static FGameplayTag State_KnockedDown;
	static FGameplayTag State_Attacking;
	static FGameplayTag State_Blocking;
	static FGameplayTag State_Dodging;
	static FGameplayTag State_Jumping;
	static FGameplayTag State_Sprinting;
	static FGameplayTag State_Invulnerable;

	// ----- Statuses -----

	// Buff Statuses
	static FGameplayTag State_Buff;
	static FGameplayTag State_Buff_AttackSpeed;
	static FGameplayTag State_Buff_Defense;

	// Debuff Statuses
	static FGameplayTag State_Debuff;
	static FGameplayTag State_Debuff_Slow;
	static FGameplayTag State_Debuff_Poisoned;
	static FGameplayTag State_Debuff_Stunned;

	// Immunity Statuses
	static FGameplayTag State_Immunity;
	static FGameplayTag State_Immunity_Stun;
	static FGameplayTag State_Immunity_Poison;
	static FGameplayTag State_Immunity_Slow;
	static FGameplayTag State_Immunity_Damage;


	// ----- Events -----

	// Health Events
	static FGameplayTag Event_Hit;
	static FGameplayTag Event_Damage_Taken;
	static FGameplayTag Event_Heal_Received;

	// Stamina events
	static FGameplayTag Event_Stamina_Consumed;
	static FGameplayTag Event_Stamina_Gained;

	// Combo Events
	static FGameplayTag Event_ComboWindow_Open;
	static FGameplayTag Event_ComboWindow_Close;

	// Ability Events
	static FGameplayTag Event_Ability_Activated;
	static FGameplayTag Event_Ability_Ended;

	// Weapon Events
	static FGameplayTag Event_Weapon_Equip;
	static FGameplayTag Event_Weapon_Equip_Next;
	static FGameplayTag Event_Weapon_Equip_Previous;

	// ----- Data -----

	// Health Data
	static FGameplayTag Data_Health_ConsumptionAmount;
	static FGameplayTag Data_Health_GainedAmount;

	// Stamina Data
	static FGameplayTag Data_Stamina_ConsumptionAmount;
	static FGameplayTag Data_Stamina_GainedAmount;

	// Mana Data
	static FGameplayTag Data_Mana_ConsumptionAmount;
	static FGameplayTag Data_Mana_GainedAmount;

	// Damage Types Data
	static FGameplayTag Data_DamageType_Physical;
	static FGameplayTag Data_DamageType_Magic;
	static FGameplayTag Data_DamageType_Fire;
	static FGameplayTag Data_DamageType_Ice;
	static FGameplayTag Data_DamageType_Lightning;
	static FGameplayTag Data_DamageType_Poison;
	static FGameplayTag Data_DamageType_Curse;


	// ----- Combos -----

	static FGameplayTag Combo_Window_LightAttack;
	static FGameplayTag Combo_Window_HeavyAttack;
	static FGameplayTag Combo_Window_Finisher;

	// ----- Attacks -----

	static FGameplayTag Attack_Type_Melee;
	static FGameplayTag Attack_Type_Ranged;
	static FGameplayTag Attack_Type_Magic;

	// ----- Effects -----

	static FGameplayTag Effect_InitAttributes;

	// ----- Cooldowns -----

	static FGameplayTag Cooldown_Healing;

	// ----- Initialize must run after the tags declarations -----
	static void InitializeNativeTags();
};
