using UnrealBuildTool;

public class Gameku : ModuleRules
{
    public Gameku(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // Tambah module root agar subfolder include (Core/, Fishing/, dll) bisa resolve
        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "UMG",
            "Slate",
            "SlateCore",
            "GameplayTags",
            "GameplayTasks",
            "AIModule",
            "NavigationSystem",
            "PhysicsCore",
            "Chaos",
            "Water"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "MoviePlayer"
        });
    }
}
