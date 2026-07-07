# 3dengine

A 3D engine written in C++ with OpenGL, built around its own editor rather than being a bare rendering library. It has a scene/object model, a camera, mesh/model loading (via Assimp), shaders, and a framebuffer-based viewport, plus embedded Lua scripting (via sol2) for driving object behavior at runtime.

<!-- TODO: editor screenshot -->

## Features

- OpenGL 3.3 renderer: shader, mesh, model (Assimp-based import), camera, and framebuffer systems, with a scene graph of objects
- Editor UI (Dear ImGui): menu bar, console window (captures engine/script output), scene hierarchy, viewport rendered into the framebuffer texture, and a script viewer with a syntax-highlighted text editor
- Project system: create or open a project (`.3dproj`) from a startup screen; scenes (including camera and object state) are serialized to/from JSON (`.3dscene`)
- Embedded Lua scripting: scripts can be attached to objects and get `init()`/`update()` lifecycle hooks, an `Object` API (position, rotation, scale, name) and a `vec3` type exposed to Lua, and `print()` output is captured into the in-editor console
- Diagnostics window (currently a placeholder)

## Status

Long-running personal project, developed on and off in spare time. Work in progress - expect rough edges and missing features (e.g. the diagnostics window and some menu items are stubs).

## Build

Required to build:

- GPU that supports OpenGL 3.3
- cmake (>= 3.10)

To build and run:

Linux/MacOS:

- `./build.sh`

Windows:

- `build.bat`

The compiled executable will be in the `build` directory.

If you're using Visual Studio Code, you have to configure include paths in c_cpp_properties.json as well. (for intellisense to work correctly, doesn't affect compilation)
