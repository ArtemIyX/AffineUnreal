<div align="center">
  <img width="1280" height="640" alt="AffinePreview_low" src="https://github.com/user-attachments/assets/bd19d64d-1021-44f4-9db0-1fdfcd59d991" />
  <h1>AffineUnreal</h1>
  <p><strong>AffineUI, bundled for Unreal Engine.</strong></p>
  <p>Explore a native C++ HTML and CSS renderer inside an Unreal plugin.</p>
  <p>
    <a href="https://github.com/ArtemIyX/AffineUnreal/stargazers"><img src="https://img.shields.io/github/stars/ArtemIyX/AffineUnreal?style=for-the-badge&logo=github&label=Stars" alt="GitHub stars" /></a>
    <a href="https://github.com/ArtemIyX/AffineUnreal/network/members"><img src="https://img.shields.io/github/forks/ArtemIyX/AffineUnreal?style=for-the-badge&logo=github&label=Forks" alt="GitHub forks" /></a>
    <a href="https://github.com/affineui/affineui"><img src="https://img.shields.io/badge/Powered%20by-AffineUI-39c5bb?style=for-the-badge" alt="Powered by AffineUI" /></a>
  </p>
</div>

---

> **Current state:** This repository bundles AffineUI's C++ renderer as an Unreal runtime plugin. A documented Unreal-facing API, UMG/Slate bridge, and runnable Unreal example are not included yet. The plugin is not currently a drop-in way to display HTML in a game or editor.

## What is AffineUI?

[AffineUI](https://github.com/affineui/affineui) is a C++ HTML/CSS UI renderer. This repository packages its generated amalgamated header and implementation in an Unreal plugin module, providing a starting point for bringing that renderer into Unreal projects.

The bundled AffineUI sources describe a native rendering engine with HTML and CSS layout/rendering capabilities. See the [upstream project](https://github.com/affineui/affineui) for its feature coverage, demos, documentation, and current status.

## What's included

- Unreal runtime plugin descriptor and `AffineUnreal` module.
- Bundled AffineUI amalgamated sources: `affineui.h` and `affineui.cpp`.
- Windows build rules configured for the D3D11 graphics backend.

## Installation

For source-level evaluation, copy or clone this repository into your Unreal project's `Plugins/AffineUnreal` directory, then regenerate project files and build the project with UnrealBuildTool.

```text
YourProject/
└── Plugins/
    └── AffineUnreal/
        ├── AffineUnreal.uplugin
        ├── Resources/
        └── Source/
```

You can enable the plugin in **Edit → Plugins** after opening the project. This installs and builds the module; it does not provide a supported UI display workflow yet. There is currently no Unreal usage snippet to show without implying an integration API that this repository does not implement.

## Compatibility

| Item | Current repository configuration |
| --- | --- |
| Unreal module type | Runtime |
| Verified Unreal Engine version | Not specified in this repository |
| Configured target platform | Windows 64-bit (`Win64`) |
| Configured graphics backend | Direct3D 11 |
| Direct3D 12 | Explicitly rejected by the current build rules |
| Other platform/backend support | Not established by this repository |

The plugin's build rules select D3D11 on Win64 and link the Windows D3D11/DXGI system libraries. Engine version and packaged-game compatibility have not been documented here; treat them as unverified until tested and recorded.

## Project status

This is an early integration package. The module currently has no Unreal-facing renderer lifecycle, input/event bridge, texture presentation path, or example actor/widget. Contributions that establish and demonstrate those pieces will make the plugin useful to Unreal developers.

## Upstream project and licensing

- [AffineUI source and documentation](https://github.com/affineui/affineui)
- [AffineUI issues](https://github.com/affineui/affineui/issues)
- [Plugin license](LICENSE)
- [AffineUI license and third-party notices](https://github.com/affineui/affineui#license)

The bundled AffineUI files note that third-party components retain their own licenses. Review the upstream notices alongside this plugin's license before redistribution.
