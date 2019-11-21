// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Blitzball : ModuleRules
{
	public Blitzball(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "UMG"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { 
            "Slate", 
            "SlateCore", 
            "OnlineSubsystem" 
        });
	}
}
