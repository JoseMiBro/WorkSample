// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
public class Pestilence : ModuleRules
{
	public Pestilence(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//Se puede añadir "Pestilence" en aqui para que añada el directorio al crear cosas
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AIModule", "GameplayTasks", "UMG", "NavigationSystem", "Niagara", "SmartObjectsModule", "GameplayBehaviorsModule", "GameplayBehaviorSmartObjectsModule", "MoviePlayer", "SkeletalMerging" });
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}