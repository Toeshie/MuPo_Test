// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class D3_Project_MuPo : ModuleRules
{
	public D3_Project_MuPo(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AITestSuite"});
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "SlateCore", "Slate" });
	}
}
