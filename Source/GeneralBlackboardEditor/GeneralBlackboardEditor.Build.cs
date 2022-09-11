// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

using UnrealBuildTool;

public class GeneralBlackboardEditor : ModuleRules
{
	public GeneralBlackboardEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
				
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"GeneralBlackboard",

				"CoreUObject",
				"Engine",
				"SlateCore",
				"Slate",
				"EditorStyle",
				"EditorWidgets",
				//"DeveloperSettings",


				"PropertyEditor",
				"ClassViewer",
				"UnrealEd",
				"Projects"
								
			
			}
			);
		
	}
}
