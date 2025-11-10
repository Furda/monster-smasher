// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MonsterSmasher : ModuleRules
{
	public MonsterSmasher(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"GameplayTasks",
			"GameplayAbilities",
			"GameplayTags",
			"ModularGameplayActors"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"MonsterSmasher",
			"MonsterSmasher/Variant_Platforming",
			"MonsterSmasher/Variant_Combat",
			"MonsterSmasher/Variant_Combat/AI",
			"MonsterSmasher/Variant_SideScrolling",
			"MonsterSmasher/Variant_SideScrolling/Gameplay",
			"MonsterSmasher/Variant_SideScrolling/AI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
