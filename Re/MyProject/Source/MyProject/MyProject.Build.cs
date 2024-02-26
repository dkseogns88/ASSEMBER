// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyProject : ModuleRules
{
    public MyProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "EnhancedInput", "Networking" });

        PrivateDependencyModuleNames.AddRange(new string[] { "ProtobufCore" });

        PrivateIncludePaths.AddRange(new string[]
        {
            "MyProject/",
            "MyProject/Network/",
            "MyProject/Game/",
        });
    }
}
