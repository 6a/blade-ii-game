// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BladeIIGame : ModuleRules
{
	public BladeIIGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "RHI" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "Json", "JsonUtilities" });

		// Websocket plugin
		PrivateDependencyModuleNames.AddRange(new string[] { "WebSocket" });
	}
}
