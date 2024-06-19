// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class A1 : ModuleRules
{
	public A1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicIncludePaths.AddRange(new string[]
        {
            "A1",

        });

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", "Sockets", "EnhancedInput", "Networking", "UMG", "Slate", "SlateCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore"});

		bEnableExceptions = true;

		
	}
}
