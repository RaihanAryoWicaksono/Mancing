using UnrealBuildTool;
using System.Collections.Generic;

public class GamekuTarget : TargetRules
{
    public GamekuTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Gameku");
    }
}
