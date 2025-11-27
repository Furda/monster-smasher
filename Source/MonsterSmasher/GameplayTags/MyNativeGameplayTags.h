#pragma once

#include "NativeGameplayTags.h"


// ----- InputTags -----

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Jump);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Dodge);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Sprint);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Interact);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Attack_Light);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Attack_Heavy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Block);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_TargetLock);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_EquipWeapon);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_UnequipWeapon);

// Ability input tags
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_InputTag_Ability1);

// ----- Abilities -----

// General Attack Abilities
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack_Light);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack_Heavy);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack_Ranged);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Attack_Special);

// Defense Abilities
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Defense_Block);

// Utility Abilities
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Utility_EquipWeapon);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Utility_UnequipWeapon);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Utility_Heal);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Utility_Buff);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Utility_Interact);

// Movement Abilities
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Movement_Sprint);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Ability_Movement_Dodge);

// ----- Weapons -----

// Unarmed
UE_DECLARE_GAMEPLAY_TAG_EXTERN(Weapon_Type_Unarmed)

// Melee Weapons
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Type_Sword);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Type_Axe);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Type_Fist);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Type_Spear);

// Range Weapons
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Type_Bow);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Weapon_Type_MagicStaff);

// ----- States -----

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Dead);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Stunned);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_KnockedDown);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Attacking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Blocking);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Dodging);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Jumping);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Sprinting);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Invulnerable);

// ----- Statuses -----

// Buff Statuses
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Buff);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Buff_AttackSpeed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Buff_Defense);

// Debuff Statuses
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Debuff);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Debuff_Slow);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Debuff_Poisoned);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Debuff_Stunned);

// Immunity Statuses
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Immunity);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Immunity_Stun);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Immunity_Poison);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Immunity_Slow);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_State_Immunity_Damage);

// ----- Events -----

// Health Events
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Hit);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Damage_Taken);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Heal_Received);

// Stamina events
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Stamina_Consumed);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Stamina_Gained);

// Combo Events
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_ComboWindow_Open);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_ComboWindow_Close);

// Ability Events
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Ability_Activated);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Ability_Ended);

// Weapon Events
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Weapon_Equip);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Weapon_Equip_Next);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Event_Weapon_Equip_Previous);

// ----- Data -----

// Health Data
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Health_ConsumptionAmount);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Health_GainedAmount);

// Stamina Data
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Stamina_ConsumptionAmount);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Stamina_GainedAmount);

// Mana Data
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Mana_ConsumptionAmount);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_Mana_GainedAmount);

// Damage Types Data
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_DamageType_Physical);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_DamageType_Magic);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_DamageType_Fire);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_DamageType_Ice);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_DamageType_Lightning);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_DamageType_Poison);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Data_DamageType_Curse);

// ----- Combos -----

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combo_Window_LightAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combo_Window_HeavyAttack);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Combo_Window_Finisher);

// ----- Attacks -----

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attack_Type_Melee);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attack_Type_Ranged);
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Attack_Type_Magic);

// ----- Effects -----

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Effect_InitAttributes);

// ----- Cooldowns -----

UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Cooldown_Healing);
