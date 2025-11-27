#include "FGameplayTags.h"
#include "GameplayTagsManager.h"

// =========================================================
// STATIC MEMBER DEFINITIONS
// =========================================================

// --- InputTags ---
FGameplayTag FGameplayTags::InputTag_Jump;
FGameplayTag FGameplayTags::InputTag_Dodge;
FGameplayTag FGameplayTags::InputTag_Sprint;
FGameplayTag FGameplayTags::InputTag_Interact;
FGameplayTag FGameplayTags::InputTag_Attack_Light;
FGameplayTag FGameplayTags::InputTag_Attack_Heavy;
FGameplayTag FGameplayTags::InputTag_Block;
FGameplayTag FGameplayTags::InputTag_TargetLock;
FGameplayTag FGameplayTags::InputTag_EquipWeapon;
FGameplayTag FGameplayTags::InputTag_Ability1;

// --- Abilities ---
FGameplayTag FGameplayTags::Ability_Attack_Light;
FGameplayTag FGameplayTags::Ability_Attack_Heavy;
FGameplayTag FGameplayTags::Ability_Attack_Ranged;
FGameplayTag FGameplayTags::Ability_Attack_Special;
FGameplayTag FGameplayTags::Ability_Defense_Block;
FGameplayTag FGameplayTags::Ability_Utility_EquipWeapon;
FGameplayTag FGameplayTags::Ability_Utility_Heal;
FGameplayTag FGameplayTags::Ability_Utility_Buff;
FGameplayTag FGameplayTags::Ability_Utility_Interact;
FGameplayTag FGameplayTags::Ability_Movement_Sprint;
FGameplayTag FGameplayTags::Ability_Movement_Dodge;

// --- Weapons ---
FGameplayTag FGameplayTags::Weapon_Type_Sword;
FGameplayTag FGameplayTags::Weapon_Type_Axe;
FGameplayTag FGameplayTags::Weapon_Type_Fist;
FGameplayTag FGameplayTags::Weapon_Type_Bow;
FGameplayTag FGameplayTags::Weapon_Type_MagicStaff;

// --- States ---
FGameplayTag FGameplayTags::State_Dead;
FGameplayTag FGameplayTags::State_Stunned;
FGameplayTag FGameplayTags::State_KnockedDown;
FGameplayTag FGameplayTags::State_Attacking;
FGameplayTag FGameplayTags::State_Blocking;
FGameplayTag FGameplayTags::State_Dodging;
FGameplayTag FGameplayTags::State_Jumping;
FGameplayTag FGameplayTags::State_Sprinting;
FGameplayTag FGameplayTags::State_Invulnerable;

// --- Statuses (Buffs/Debuffs/Immunities) ---
FGameplayTag FGameplayTags::State_Buff;
FGameplayTag FGameplayTags::State_Buff_AttackSpeed;
FGameplayTag FGameplayTags::State_Buff_Defense;
FGameplayTag FGameplayTags::State_Debuff;
FGameplayTag FGameplayTags::State_Debuff_Slow;
FGameplayTag FGameplayTags::State_Debuff_Poisoned;
FGameplayTag FGameplayTags::State_Debuff_Stunned;
FGameplayTag FGameplayTags::State_Immunity;
FGameplayTag FGameplayTags::State_Immunity_Stun;
FGameplayTag FGameplayTags::State_Immunity_Poison;
FGameplayTag FGameplayTags::State_Immunity_Slow;
FGameplayTag FGameplayTags::State_Immunity_Damage;

// --- Events ---
FGameplayTag FGameplayTags::Event_Hit;
FGameplayTag FGameplayTags::Event_Damage_Taken;
FGameplayTag FGameplayTags::Event_Heal_Received;
FGameplayTag FGameplayTags::Event_Stamina_Consumed;
FGameplayTag FGameplayTags::Event_Stamina_Gained;
FGameplayTag FGameplayTags::Event_ComboWindow_Open;
FGameplayTag FGameplayTags::Event_ComboWindow_Close;
FGameplayTag FGameplayTags::Event_Ability_Activated;
FGameplayTag FGameplayTags::Event_Ability_Ended;
FGameplayTag FGameplayTags::Event_Weapon_Equip;
FGameplayTag FGameplayTags::Event_Weapon_Equip_Next;
FGameplayTag FGameplayTags::Event_Weapon_Equip_Previous;

// --- Data ---
FGameplayTag FGameplayTags::Data_Health_ConsumptionAmount;
FGameplayTag FGameplayTags::Data_Health_GainedAmount;
FGameplayTag FGameplayTags::Data_Stamina_ConsumptionAmount;
FGameplayTag FGameplayTags::Data_Stamina_GainedAmount;
FGameplayTag FGameplayTags::Data_Mana_ConsumptionAmount;
FGameplayTag FGameplayTags::Data_Mana_GainedAmount;
FGameplayTag FGameplayTags::Data_DamageType_Physical;
FGameplayTag FGameplayTags::Data_DamageType_Magic;
FGameplayTag FGameplayTags::Data_DamageType_Fire;
FGameplayTag FGameplayTags::Data_DamageType_Ice;
FGameplayTag FGameplayTags::Data_DamageType_Lightning;
FGameplayTag FGameplayTags::Data_DamageType_Poison;
FGameplayTag FGameplayTags::Data_DamageType_Curse;

// --- Combos ---
FGameplayTag FGameplayTags::Combo_Window_LightAttack;
FGameplayTag FGameplayTags::Combo_Window_HeavyAttack;
FGameplayTag FGameplayTags::Combo_Window_Finisher;

// --- Attacks ---
FGameplayTag FGameplayTags::Attack_Type_Melee;
FGameplayTag FGameplayTags::Attack_Type_Ranged;
FGameplayTag FGameplayTags::Attack_Type_Magic;

// --- Effects ---
FGameplayTag FGameplayTags::Effect_InitAttributes;

// --- Cooldowns ---
FGameplayTag FGameplayTags::Cooldown_Healing;


// =========================================================
// INITIALIZATION LOGIC
// =========================================================

void FGameplayTags::InitializeNativeTags()
{
	// The Manager call is safe here because this is done early in engine initialization.

	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	// --- Input Tags ---
	InputTag_Jump = Manager.RequestGameplayTag(FName("Input.Jump"));
	InputTag_Dodge = Manager.RequestGameplayTag(FName("Input.Dodge"));
	InputTag_Sprint = Manager.RequestGameplayTag(FName("Input.Sprint"));
	InputTag_Interact = Manager.RequestGameplayTag(FName("Input.Interact"));
	InputTag_Attack_Light = Manager.RequestGameplayTag(FName("Input.Attack.Light"));
	InputTag_Attack_Heavy = Manager.RequestGameplayTag(FName("Input.Attack.Heavy"));
	InputTag_Block = Manager.RequestGameplayTag(FName("Input.Block"));
	InputTag_TargetLock = Manager.RequestGameplayTag(FName("Input.TargetLock"));
	InputTag_EquipWeapon = Manager.RequestGameplayTag(FName("Input.EquipWeapon"));
	InputTag_Ability1 = Manager.RequestGameplayTag(FName("Input.Ability1"));

	// --- Ability Tags ---
	// General Attack Abilities
	Ability_Attack_Light = Manager.RequestGameplayTag(FName("Ability.Attack.Light"));
	Ability_Attack_Heavy = Manager.RequestGameplayTag(FName("Ability.Attack.Heavy"));
	Ability_Attack_Ranged = Manager.RequestGameplayTag(FName("Ability.Attack.Ranged"));
	Ability_Attack_Special = Manager.RequestGameplayTag(FName("Ability.Attack.Special"));
	// Defense Abilities
	Ability_Defense_Block = Manager.RequestGameplayTag(FName("Ability.Defense.Block"));
	// Utility Abilities
	Ability_Utility_EquipWeapon = Manager.RequestGameplayTag(FName("Ability.Utility.EquipWeapon"));
	Ability_Utility_Heal = Manager.RequestGameplayTag(FName("Ability.Utility.Heal"));
	Ability_Utility_Buff = Manager.RequestGameplayTag(FName("Ability.Utility.Buff"));
	Ability_Utility_Interact = Manager.RequestGameplayTag(FName("Ability.Utility.Interact"));
	// Movement Abilities
	Ability_Movement_Sprint = Manager.RequestGameplayTag(FName("Ability.Movement.Sprint"));
	Ability_Movement_Dodge = Manager.RequestGameplayTag(FName("Ability.Movement.Dodge"));

	// --- Weapon Tags ---
	// Melee Weapons
	Weapon_Type_Sword = Manager.RequestGameplayTag(FName("Weapon.Type.Sword"));
	Weapon_Type_Axe = Manager.RequestGameplayTag(FName("Weapon.Type.Axe"));
	Weapon_Type_Fist = Manager.RequestGameplayTag(FName("Weapon.Type.Fist"));
	// Range Weapons
	Weapon_Type_Bow = Manager.RequestGameplayTag(FName("Weapon.Type.Bow"));
	// Note: Renamed "Weapon_Type_MagicStaff" to "Weapon_Ranged_MagicStaff" for consistency
	Weapon_Type_MagicStaff = Manager.RequestGameplayTag(FName("Weapon.Type.MagicStaff"));

	// --- State Tags ---
	State_Dead = Manager.RequestGameplayTag(FName("State.Dead"));
	State_Stunned = Manager.RequestGameplayTag(FName("State.Stunned"));
	State_KnockedDown = Manager.RequestGameplayTag(FName("State.KnockedDown"));
	State_Attacking = Manager.RequestGameplayTag(FName("State.Attacking"));
	State_Blocking = Manager.RequestGameplayTag(FName("State.Blocking"));
	State_Dodging = Manager.RequestGameplayTag(FName("State.Dodging"));
	State_Jumping = Manager.RequestGameplayTag(FName("State.Jumping"));
	State_Sprinting = Manager.RequestGameplayTag(FName("State.Sprinting"));
	State_Invulnerable = Manager.RequestGameplayTag(FName("State.Invulnerable"));

	// --- Status Tags ---
	// Buff Statuses
	State_Buff = Manager.RequestGameplayTag(FName("State.Buff"));
	State_Buff_AttackSpeed = Manager.RequestGameplayTag(FName("State.Buff.AttackSpeed"));
	State_Buff_Defense = Manager.RequestGameplayTag(FName("State.Buff.Defense"));
	// Debuff Statuses
	State_Debuff = Manager.RequestGameplayTag(FName("State.Debuff"));
	State_Debuff_Slow = Manager.RequestGameplayTag(FName("State.Debuff.Slow"));
	State_Debuff_Poisoned = Manager.RequestGameplayTag(FName("State.Debuff.Poisoned"));
	State_Debuff_Stunned = Manager.RequestGameplayTag(FName("State.Debuff.Stunned"));
	// Immunity Statuses
	State_Immunity = Manager.RequestGameplayTag(FName("State.Immunity"));
	State_Immunity_Stun = Manager.RequestGameplayTag(FName("State.Immunity.Stun"));
	State_Immunity_Poison = Manager.RequestGameplayTag(FName("State.Immunity.Poison"));
	State_Immunity_Slow = Manager.RequestGameplayTag(FName("State.Immunity.Slow"));
	State_Immunity_Damage = Manager.RequestGameplayTag(FName("State.Immunity.Damage"));

	// --- Event Tags ---
	// Health Events
	Event_Hit = Manager.RequestGameplayTag(FName("Event.Hit"));
	Event_Damage_Taken = Manager.RequestGameplayTag(FName("Event.Damage.Taken"));
	Event_Heal_Received = Manager.RequestGameplayTag(FName("Event.Heal.Received"));
	// Stamina events
	Event_Stamina_Consumed = Manager.RequestGameplayTag(FName("Event.Stamina.Consumed"));
	Event_Stamina_Gained = Manager.RequestGameplayTag(FName("Event.Stamina.Gained"));
	// Combo Events
	Event_ComboWindow_Open = Manager.RequestGameplayTag(FName("Event.ComboWindow.Open"));
	Event_ComboWindow_Close = Manager.RequestGameplayTag(FName("Event.ComboWindow.Close"));
	// Ability Events
	Event_Ability_Activated = Manager.RequestGameplayTag(FName("Event.Ability.Activated"));
	Event_Ability_Ended = Manager.RequestGameplayTag(FName("Event.Ability.Ended"));
	// Weapon Events
	Event_Weapon_Equip = Manager.RequestGameplayTag(FName("Event.Weapon.Equip"));
	Event_Weapon_Equip_Next = Manager.RequestGameplayTag(FName("Event.Weapon.Equip.Next"));
	Event_Weapon_Equip_Previous = Manager.RequestGameplayTag(FName("Event.Weapon.Equip.Previous"));


	// --- Data Tags ---
	// Health Data
	Data_Health_ConsumptionAmount = Manager.RequestGameplayTag(
		FName("Data.Health.ConsumptionAmount"));
	Data_Health_GainedAmount = Manager.RequestGameplayTag(FName("Data.Health.GainedAmount"));
	// Stamina Data
	Data_Stamina_ConsumptionAmount = Manager.RequestGameplayTag(
		FName("Data.Stamina.ConsumptionAmount"));
	Data_Stamina_GainedAmount = Manager.RequestGameplayTag(FName("Data.Stamina.GainedAmount"));
	// Mana Data
	Data_Mana_ConsumptionAmount = Manager.RequestGameplayTag(FName("Data.Mana.ConsumptionAmount"));
	Data_Mana_GainedAmount = Manager.RequestGameplayTag(FName("Data.Mana.GainedAmount"));
	// Damage Types Data
	Data_DamageType_Physical = Manager.RequestGameplayTag(FName("Data.DamageType.Physical"));
	Data_DamageType_Magic = Manager.RequestGameplayTag(FName("Data.DamageType.Magic"));
	Data_DamageType_Fire = Manager.RequestGameplayTag(FName("Data.DamageType.Fire"));
	Data_DamageType_Ice = Manager.RequestGameplayTag(FName("Data.DamageType.Ice"));
	Data_DamageType_Lightning = Manager.RequestGameplayTag(FName("Data.DamageType.Lightning"));
	Data_DamageType_Poison = Manager.RequestGameplayTag(FName("Data.DamageType.Poison"));
	Data_DamageType_Curse = Manager.RequestGameplayTag(FName("Data.DamageType.Curse"));

	// --- Combo Tags ---
	Combo_Window_LightAttack = Manager.RequestGameplayTag(FName("Combo.Window.LightAttack"));
	Combo_Window_HeavyAttack = Manager.RequestGameplayTag(FName("Combo.Window.HeavyAttack"));
	Combo_Window_Finisher = Manager.RequestGameplayTag(FName("Combo.Window.Finisher"));

	// --- Attack Tags ---
	Attack_Type_Melee = Manager.RequestGameplayTag(FName("Attack.Type.Melee"));
	Attack_Type_Ranged = Manager.RequestGameplayTag(FName("Attack.Type.Ranged"));
	Attack_Type_Magic = Manager.RequestGameplayTag(FName("Attack.Type.Magic"));

	// --- Effect Tags ---
	Effect_InitAttributes = Manager.RequestGameplayTag(FName("Effect.InitAttributes"));

	// --- Cooldown Tags ---
	Cooldown_Healing = Manager.RequestGameplayTag(FName("Cooldown.Healing"));
}
