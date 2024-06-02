// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject : ModuleRules
{
    public MyProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "EnhancedInput", "Networking", "UMG", "Slate", "SlateCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });

        bEnableExceptions = true;

        PrivateIncludePaths.AddRange(new string[]
        {
            "MyProject/",
            "MyProject/Network/",
            "MyProject/Character/",
        });
    }
}
