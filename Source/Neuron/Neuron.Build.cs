// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Neuron : ModuleRules
{
	public Neuron(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput", "LootLockerSDK" });
		PrivateDependencyModuleNames.AddRange(new string[] { "HTTP", "Json", "JsonUtilities" }); // for LootLockerSDK
	}
}
