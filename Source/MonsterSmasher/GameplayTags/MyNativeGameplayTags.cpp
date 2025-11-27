#include "MyNativeGameplayTags.h"

// ----- InputTags -----

UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Jump, "InputTag.Jump");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Dodge, "InputTag.Dodge");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Sprint, "InputTag.Sprint");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Interact, "InputTag.Interact");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Attack_Light, "InputTag.Attack.Light");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Attack_Heavy, "InputTag.Attack.Heavy");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Block, "InputTag.Block");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_TargetLock, "InputTag.TargetLock");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_EquipWeapon, "InputTag.EquipWeapon");
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_UnequipWeapon, "InputTag.UnequipWeapon");


// Ability input tags
UE_DEFINE_GAMEPLAY_TAG(TAG_InputTag_Ability1, "InputTag.Ability1");

// ----- Abilities -----

// General Attack Abilities
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack_Light, "Ability.Attack.Light");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack_Heavy, "Ability.Attack.Heavy");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack_Ranged, "Ability.Attack.Ranged");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Attack_Special, "Ability.Attack.Special");

// Defense Abilities
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Defense_Block, "Ability.Defense.Block");

// Utility Abilities
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Utility_EquipWeapon, "Ability.Utility.EquipWeapon");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Utility_UnequipWeapon, "Ability.Utility.UnequipWeapon");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Utility_Heal, "Ability.Utility.Heal");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Utility_Buff, "Ability.Utility.Buff");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Utility_Interact, "Ability.Utility.Interact");

// Movement Abilities
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Movement_Sprint, "Ability.Movement.Sprint");
UE_DEFINE_GAMEPLAY_TAG(TAG_Ability_Movement_Dodge, "Ability.Movement.Dodge");

// ----- Weapons -----

// Unarmed 
UE_DEFINE_GAMEPLAY_TAG(Weapon_Type_Unarmed, "Weapon.Type.Unarmed");

// Melee Weapons
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Type_Sword, "Weapon.Type.Sword");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Type_Axe, "Weapon.Type.Axe");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Type_Fist, "Weapon.Type.Fist");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Type_Spear, "Weapon.Type.Spear");

// Range Weapons
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Type_Bow, "Weapon.Type.Bow");
UE_DEFINE_GAMEPLAY_TAG(TAG_Weapon_Type_MagicStaff, "Weapon.Type.MagicStaff");

// ----- States -----

UE_DEFINE_GAMEPLAY_TAG(TAG_State_Dead, "State.Dead");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Stunned, "State.Stunned");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_KnockedDown, "State.KnockedDown");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Attacking, "State.Attacking");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Blocking, "State.Blocking");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Dodging, "State.Dodging");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Jumping, "State.Jumping");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Sprinting, "State.Sprinting");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Invulnerable, "State.Invulnerable");

// ----- Statuses -----

// Buff Statuses
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Buff, "State.Buff");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Buff_AttackSpeed, "State.Buff.AttackSpeed");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Buff_Defense, "State.Buff.Defense");

// Debuff Statuses
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Debuff, "State.Debuff");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Debuff_Slow, "State.Debuff.Slow");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Debuff_Poisoned, "State.Debuff.Poisoned");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Debuff_Stunned, "State.Debuff.Stunned");

// Immunity Statuses
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Immunity, "State.Immunity");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Immunity_Stun, "State.Immunity.Stun");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Immunity_Poison, "State.Immunity.Poison");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Immunity_Slow, "State.Immunity.Slow");
UE_DEFINE_GAMEPLAY_TAG(TAG_State_Immunity_Damage, "State.Immunity.Damage");

// ----- Events -----

// Health Events
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Hit, "Event.Hit");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Damage_Taken, "Event.Damage.Taken");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Heal_Received, "Event.Heal.Received");

// Stamina events
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Stamina_Consumed, "Event.Stamina.Consumed");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Stamina_Gained, "Event.Stamina.Gained");

// Combo Events
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_ComboWindow_Open, "Event.ComboWindow.Open");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_ComboWindow_Close, "Event.ComboWindow.Close");

// Ability Events
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Ability_Activated, "Event.Ability.Activated");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Ability_Ended, "Event.Ability.Ended");

// Weapon Events
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Weapon_Equip, "Event.Weapon.Equip");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Weapon_Equip_Next, "Event.Weapon.Equip.Next");
UE_DEFINE_GAMEPLAY_TAG(TAG_Event_Weapon_Equip_Previous, "Event.Weapon.Equip.Previous");

// ----- Data -----

// Health Data
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Health_ConsumptionAmount, "Data.Health.ConsumptionAmount");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Health_GainedAmount, "Data.Health.GainedAmount");

// Stamina Data
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Stamina_ConsumptionAmount, "Data.Stamina.ConsumptionAmount");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Stamina_GainedAmount, "Data.Stamina.GainedAmount");

// Mana Data
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Mana_ConsumptionAmount, "Data.Mana.ConsumptionAmount");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_Mana_GainedAmount, "Data.Mana.GainedAmount");

// Damage Types Data
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_DamageType_Physical, "Data.DamageType.Physical");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_DamageType_Magic, "Data.DamageType.Magic");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_DamageType_Fire, "Data.DamageType.Fire");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_DamageType_Ice, "Data.DamageType.Ice");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_DamageType_Lightning, "Data.DamageType.Lightning");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_DamageType_Poison, "Data.DamageType.Poison");
UE_DEFINE_GAMEPLAY_TAG(TAG_Data_DamageType_Curse, "Data.DamageType.Curse");

// ----- Combos -----

UE_DEFINE_GAMEPLAY_TAG(TAG_Combo_Window_LightAttack, "Combo.Window.LightAttack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Combo_Window_HeavyAttack, "Combo.Window.HeavyAttack");
UE_DEFINE_GAMEPLAY_TAG(TAG_Combo_Window_Finisher, "Combo.Window.Finisher");

// ----- Attacks -----

UE_DEFINE_GAMEPLAY_TAG(TAG_Attack_Type_Melee, "Attack.Type.Melee");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attack_Type_Ranged, "Attack.Type.Ranged");
UE_DEFINE_GAMEPLAY_TAG(TAG_Attack_Type_Magic, "Attack.Type.Magic");

// ----- Effects -----

UE_DEFINE_GAMEPLAY_TAG(TAG_Effect_InitAttributes, "Effect.InitAttributes");

// ----- Cooldowns -----

UE_DEFINE_GAMEPLAY_TAG(TAG_Cooldown_Healing, "Cooldown.Healing");
