using UnrealBuildTool;

public class AshfallProvincesEditorTarget : TargetRules
{
    public AshfallProvincesEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("AshfallProvinces");
    }
}
