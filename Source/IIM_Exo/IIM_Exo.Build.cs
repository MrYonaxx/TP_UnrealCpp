// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class IIM_Exo : ModuleRules
{
	public IIM_Exo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
