# Counter-Strike 2 External DirectX 11 Overlay & Windows Internals Study

An external process memory analysis tool and transparent hardware-accelerated overlay written in **C++17** for Counter-Strike 2. This project explores Windows process and handle management, dynamically resolved NT Native API functions, Direct3D 11 rendering, and the transfer of sampled entity data between a reader thread and a rendering loop.

The primary focus is practical Windows internals: locating a process and its modules, inspecting system handle information, reading remote memory, and managing the lifetime of native and graphics resources.

---

> [!WARNING]
> This software is intended solely for offline analysis, local practice environments, and private educational research. **Do not use this tool in public multiplayer sessions or ruin the experience for other players.**

---

## Technical Highlights

- **Windows Native API Integration:** Resolves `NtReadVirtualMemory`, `NtQuerySystemInformation`, and `NtDuplicateObject` from `ntdll.dll` at runtime through `GetProcAddress`.
- **System Handle Enumeration & Duplication:** Inspects system handle information, evaluates candidate handles, and checks the target process ID after duplication. Attachment depends on finding an accessible, suitable handle.
- **Process & Module Discovery:** Uses Toolhelp32 snapshots to locate `cs2.exe` and resolve the base address of `client.dll` at runtime.
- **Templated External Memory Reads:** Wraps `NtReadVirtualMemory` to read the view matrix, entity references, player state, and bone positions into local data structures.
- **Transparent DirectX 11 Overlay:** Creates a borderless, layered, topmost window using `WS_EX_LAYERED`, `WS_EX_TOPMOST`, and `WS_EX_TRANSPARENT`, with DWM frame extension and Direct3D 11 rendering.
- **Separate Sampling & Rendering:** A worker thread collects data and publishes a snapshot through a mutex-protected store. The main thread copies the latest snapshot and renders it after releasing the lock.
- **COM Resource Management:** Uses `Microsoft::WRL::ComPtr` for Direct3D interfaces. Shutdown requests stop the worker and join it before the resources it uses are destroyed.
- **World-to-Screen Projection:** Transforms 3D positions using a 4x4 view-projection matrix and perspective division to obtain screen coordinates.

---

## Core Features

### Visuals (DirectX 11 ESP)

- **2D Bounding Boxes:** Screen-space boxes derived from player position and a configurable head-height approximation.
- **Skeleton Rendering:** Draws selected bone connections and a head marker from sampled bone positions.
- **Health Bars:** Displays a vertical health bar beside each rendered player box.
- **Snaplines:** Draws lines from the bottom-center of the screen to player positions.
- **Team-Based Colors:** Supports separate colors for teammates and opponents.
- **Center Crosshair:** Draws a small marker at the center of the screen.

### Interactive Menu

- Toggle bounding boxes, skeletons, and snaplines independently.
- Adjust teammate and opponent colors through Dear ImGui color editors.
- Switch the overlay between click-through rendering and menu interaction.
- Forward window messages to the Dear ImGui Win32 backend through a custom window procedure.

### Controls

| Key | Action |
| --- | --- |
| `Insert` | Show or hide the menu |
| `End` | Request shutdown and exit the application |

---

## Data Flow & Thread Lifecycle

```text
Reader thread
    |
    v
Sample remote memory and build local player data
    |
    v
Publish snapshot under a mutex
    |
    v
Main thread copies the latest snapshot under the same mutex
    |
    v
Release the lock, project coordinates, and render with ImGui / DX11
```

The shared store protects snapshot publication and copying. Drawing takes place outside the critical section. This keeps rendering work out of the reader's synchronization path; it does not make the individual remote memory reads an atomic snapshot of the target process.

The engine owns the worker thread and an atomic running flag. Shutdown clears the flag and joins the worker. Failures in required root reads request shutdown, while failed reads for individual entity candidates skip that candidate.

---

## Prerequisites & Third-Party Libraries

- **Windows x64** with a Direct3D 11-capable graphics device.
- **Visual Studio 2022** with the **Desktop development with C++** workload.
- **MSVC v143** toolset and a **Windows SDK**.
- **C++17** language support.

This source tree includes **Dear ImGui v1.92.8** and its **Win32** and **Direct3D 11** backends. The Visual Studio project already references the included sources; no separate ImGui download is needed when the complete repository is present.

Dear ImGui is developed by Omar Cornut and contributors. Its MIT license is included in [imgui-LICENSE.txt](imgui-LICENSE.txt).

---

## Building & Running

1. Open `cs2ESPExternal.sln` in Visual Studio 2022.
2. Select **Release** and **x64**.
3. Build the solution.
4. Verify that the values in `cs2ESPExternal/offsets.hpp` and the entity layout assumptions match the target build. Rebuild after changing them.
5. Start the target in a local practice environment, then launch the compiled application.
6. Press **Insert** to open the menu or **End** to exit.

The application expects the target process and its window to exist before attachment. Access permissions and the availability of a suitable process handle affect whether attachment succeeds.

---

## Project Structure

```text
.
|-- .gitignore
|-- README.md
|-- imgui-LICENSE.txt
|-- cs2ESPExternal.sln
`-- cs2ESPExternal/
    |-- Application.hpp / .cpp    # Engine initialization, main loop, and thread lifecycle
    |-- Process.hpp / .cpp        # Process/module discovery, handle analysis, and memory reads
    |-- Syscall.hpp / .cpp        # Runtime resolution of NT Native API exports
    |-- Window.hpp / .cpp         # Overlay window, message handling, and click-through state
    |-- D3D11Ctx.hpp / .cpp       # D3D11 device, context, swap chain, and render target
    |-- ImGuiUtils.hpp / .cpp     # ImGui lifecycle, drawing primitives, and menu
    |-- readData.hpp / .cpp       # Entity traversal and snapshot construction
    |-- dataMagazine.hpp / .cpp   # Mutex-protected snapshot storage
    |-- handleVisual.hpp / .cpp   # Projection and visual composition
    |-- Transform.hpp / .cpp      # World-to-screen calculations
    |-- Vec.hpp                   # Vector, player, bone, and snapshot structures
    |-- config.hpp                # Visual settings and default colors
    |-- offsets.hpp               # Compile-time offsets requiring manual maintenance
    |-- cs2ESPExternal.cpp        # Application entry point
    |-- imgui*.cpp / .h           # Bundled Dear ImGui sources
    `-- backends/                 # Dear ImGui Win32 and DX11 integration
```

---

## Additional Information

- **Static Offsets & Project Scope:** This project uses compile-time offsets, so game updates may require manual changes and a rebuild. Automatic offset downloads and pattern scanning were considered but left outside the current scope. My goal was to demonstrate Windows internals knowledge and external rendering techniques rather than maintain a continuously updated tool. I chose to document this limitation and move on to exploring internal development.

- **Progression from Earlier Projects:** My earlier projects focused primarily on implementing specific technical features to demonstrate my understanding of Windows internals. Resource ownership, error handling, and maintainable structure received less attention, which left weaknesses in their reliability and readability. Working on this project helped me recognize that these aspects are part of demonstrating technical competence: getting a feature to work is only one part of the implementation.

- **An Evolving Approach:** Much of the functionality was already implemented when I began revisiting the design. I subsequently improved responsibility boundaries, COM resource management, data synchronization, and thread shutdown. Some earlier design choices remain, and there is still room for improvement. This project represents a step toward writing code that is easier to understand, debug, and maintain, while my previous repositories document earlier stages of that learning process.
