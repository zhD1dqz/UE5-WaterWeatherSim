

using UnrealBuildTool; 

public class FlyViewer : ModuleRules
{
	public FlyViewer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
                // ... add public include paths required here ...
                "FlyViewer/Public"
			}
			);
		
		PrivateIncludePaths.AddRange(
			new string[] {
                // ... add other private include paths required here ...
                "FlyViewer/Private"
			}
			);
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                "EnhancedInput",
                "UMG"
			}
			);
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                // ... add private dependencies that you statically link with here ...
			}
			);
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
                // ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
