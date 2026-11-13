// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AffineUnreal : ModuleRules
{
	public AffineUnreal(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		// Yoga represents undefined dimensions with NaN and tests them via x != x.
		// Unreal's default fast floating-point mode can optimize that test away.
		FPSemantics = FPSemanticsMode.Precise;

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Export the existing C embedding interface to other Unreal modules.
			PrivateDefinitions.Add("AFFINEUI_C_BUILD_DLL=1");
			// Unreal owns the process entry point; do not emit Sokol's WinMain.
			PrivateDefinitions.Add("SOKOL_NO_ENTRY=1");

			// AffineUI's bundled NanoVG/Sokol shader variants support D3D11,
			// GLCORE, and the compile-only DUMMY backend on Windows. Change this
			// selector to switch the backend used by this module.
			const string backend = "D3D11";
			switch (backend)
			{
				case "D3D11":
					PublicDefinitions.Add("SOKOL_D3D11=1");
					PrivateDefinitions.Add("AFFINEUI_BACKEND_D3D11=1");
					PublicSystemLibraries.AddRange(new string[] { "d3d11.lib", "dxgi.lib" });
					break;
				case "GLCORE":
					PublicDefinitions.Add("SOKOL_GLCORE=1");
					PublicSystemLibraries.Add("opengl32.lib");
					break;
				case "DUMMY":
					PublicDefinitions.Add("SOKOL_DUMMY_BACKEND=1");
					break;
				case "D3D12":
					throw new BuildException("AffineUI's bundled Sokol/NanoVG backend does not support D3D12.");
				default:
					throw new BuildException(
						$"Unsupported AffineUI Sokol backend '{backend}'. Use D3D11, GLCORE, or DUMMY on Win64.");
			}
		}

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
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