# Ashfall Provinces — Unreal Engine 5.8 Foundation

This project is a C++-first foundation for the bounded Ashfall Provinces vertical slice. Rendering is deliberately thin; simulation state and rules live under `Source/AshfallProvinces/Public/Simulation`.

## Open the project

1. Install Unreal Engine 5.8 and a compatible Visual Studio C++ toolchain with the **Game development with C++** workload.
2. Double-click `AshfallProvinces.uproject`. If Unreal asks to rebuild missing modules, choose **Yes**.
3. If project files are required, right-click the `.uproject`, choose **Generate Visual Studio project files**, build the `AshfallProvincesEditor` Development Editor target, then reopen the project.

## Create the first test scene

1. In the Content Browser, create folders `Content/Maps` and `Content/Blueprints`.
2. Create a Basic Level and save it as `Content/Maps/L_FirstTest` (the config already points to this map).
3. Add a large floor plane, a Directional Light and a Sky Atmosphere.
4. Press Play. `AAPGameMode` spawns `AAPStrategyCameraPawn`; `UAPSimulationSubsystem` automatically creates 24 households, one settlement resource ledger and six linked provinces.
5. For the first debug Blueprint, obtain **Get World Subsystem → AP Simulation Subsystem** and display `Get Resources`, `Get Households`, `Get Companies` and `Get Simulation Tick`. Buttons can call `Muster Company`, `Order Move`, `Apply Casualties`, `Capture Province`, `Save Simulation` and `Load Simulation`; use `AshfallSlice` as the first save-slot name.

Input axis mappings for `MoveForward`, `MoveRight` and `Zoom` can be added in Project Settings → Input when the first camera-control Blueprint is created. The simulation runs without these mappings.

## Deterministic simulation

`APSimulationModel.h` is standard C++17 and has no Unreal includes. Unreal's subsystem adapts its reflected state to this model for production, movement, casualty and seeded battle calculations. The standalone test is `Tests/APSimulationModelTests.cpp`.

Example outside Unreal when a C++ compiler is installed:

```powershell
clang++ -std=c++17 Tests/APSimulationModelTests.cpp -o APSimulationModelTests.exe
./APSimulationModelTests.exe
```

Generated Unreal directories are ignored by the project-local `.gitignore`.
