// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Y : ModuleRules
{
	public Y(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "Y" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput","UMG", "NavigationSystem", "AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


	}
}
