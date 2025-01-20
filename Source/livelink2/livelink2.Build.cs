// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class livelink2 : ModuleRules
{
	public livelink2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "Sockets", "Networking", });
	}
}