// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

using UnrealBuildTool;

public class GeneralBlackboard : ModuleRules
{
	public GeneralBlackboard(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		//PrivatePCHHeaderFile = "Public/GeneralBlackboardModule.h";

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
            }
			);
		
		
	}
}
