using UnrealBuildTool;

public class AshfallProvincesTarget : TargetRules
{
    public AshfallProvincesTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("AshfallProvinces");
    }
}
