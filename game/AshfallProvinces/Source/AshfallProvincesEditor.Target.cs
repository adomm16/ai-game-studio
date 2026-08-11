using UnrealBuildTool;

public class AshfallProvincesEditorTarget : TargetRules
{
    public AshfallProvincesEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("AshfallProvinces");
    }
}
