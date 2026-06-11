using UnrealBuildTool;
using System.Collections.Generic;

public class GamekuEditorTarget : TargetRules
{
    public GamekuEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V6;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("Gameku");
    }
}
