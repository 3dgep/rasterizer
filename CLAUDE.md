# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**SoftRast** is a software rasterized graphics framework for making 2D games. This project implements a CPU-based rasterizer that can render primitives, sprites, text, and tilemaps without GPU acceleration.

## Build System

The project uses CMake with presets for different platforms:

### Configure
```bash
# Windows (Visual Studio 2022)
cmake --preset vs17

# Linux (GCC)
cmake --preset gcc
```

### Build
```bash
# Windows Debug
cmake --build --preset x64-debug

# Windows Release
cmake --build --preset x64-release

# Linux Debug
cmake --build --preset gcc-debug

# Linux Release
cmake --build --preset gcc-release
```

### Testing
Tests are disabled by default. To enable and run them:
```bash
cmake --preset gcc -DSR_BUILD_TESTS=ON
cmake --build out/build/gcc
ctest --test-dir out/build/gcc
```

## Architecture

### Core Libraries

The project is organized into two main libraries:

1. **graphics** (`graphics/` directory)
   - Software rasterization engine
   - Core rendering primitives: lines, circles, triangles, quads
   - High-level graphics objects: sprites, spritesheets, tilemaps, text
   - Image loading/saving (via stb libraries)
   - Window management (via SDL3)
   - ImGui integration for debug UI
   - Requires C++23

2. **math** (`math/` directory)
   - 2D/3D camera systems
   - Geometric primitives: AABB, Circle, Sphere, Plane, Line, Rect
   - Transform2D for 2D transformations
   - Viewport management
   - Built on GLM for vector/matrix math
   - Requires C++20

### External Dependencies

Located in `externals/`:
- SDL3 (windowing, input, audio)
- SDL_ttf (TrueType font rendering)
- GLM (math library)
- ImGui (debug UI)
- Audio library (custom audio system)
- Input library (input handling)

### Rasterizer State Machine

The `Rasterizer` class uses a state-based design. Before drawing, configure `rasterizer.state`:
- `color` - Blend color for primitives
- `fillMode` - Solid or wireframe rendering
- `cullMode` - Triangle culling (Front/Back/None)
- `frontCounterClockwise` - Winding order for front faces
- `blendMode` - How pixels are blended (important for transparency)
- `colorTarget` - Image to render to
- `viewport` - For split-screen or viewport clipping

### Key Graphics Types

- **Image**: CPU-side pixel buffer (RGBA). Supports loading from disk, sampling with different address modes, and saving to disk
- **Sprite**: A rectangle within an Image with blend mode and color modulation
- **SpriteSheet**: Collection of sprites from a single image
- **SpriteAnimation**: Animated sprite with frame sequencing
- **TileMap**: Grid-based rendering for tile-based games
- **Window**: SDL3 window wrapper that manages rendering and event polling

### Coordinate Systems

- Screen space: Integer pixel coordinates
- Image space: Can be converted from client (window) coordinates using `Window::clientToImage()`
- Texture coordinates: Either normalized [0-1] or integer pixel coordinates
- The rasterizer uses software clipping and viewport transforms

### Address Modes

When sampling textures, different address modes control out-of-bounds behavior:
- `Wrap`: Tiles the texture
- `Clamp`: Clamps to edge pixels
- `Mirror`: Mirrors the texture

## Samples

The `samples/` directory contains 16 examples demonstrating features progressively:

1. **00-Common**: Shared utilities (Timer)
2. **01-ClearScreen**: Basic window and clearing
3. **02-Images**: Image loading and display
4. **03-Triangle**: Triangle rasterization
5. **04-ImGui**: ImGui integration
6. **05-Sprites**: Sprite rendering
7. **06-FillRules**: Polygon fill modes
8. **07-TileMaps**: Tilemap rendering (uses LDtkLoader)
9. **08-Circles**: Circle rasterization
10. **09-EdgeFunction**: Edge function technique
11. **10-Barycentric**: Barycentric coordinate interpolation
12. **11-AddressModes**: Texture address modes
13. **12-Audio**: Audio playback
14. **13-Input**: Input handling
15. **14-Pong**: Complete Pong game
16. **15-PixelAdventure**: Platformer game
17. **16-Arkanoid**: Arkanoid/Breakout clone (current startup project)

Each sample links against `sr::graphics`, `sr::math`, and `00-Common`.

## Development Workflow

### Running a Sample

Build the project, then run executables from `out/build/<preset>/<configuration>/samples/<sample-name>/`.

On Windows with Visual Studio, the startup project is set to `16-Arkanoid`.

### Adding a New Sample

1. Create directory in `samples/`
2. Add `CMakeLists.txt` with executable target
3. Add `add_sample(<name>)` call to `samples/CMakeLists.txt`
4. Link against `sr::graphics`, `sr::math`, and `00-Common`

### Blend Modes

Understanding blend modes is critical for correct rendering:
- Use `BlendMode::AlphaBlend` for sprites with transparency
- Use `BlendMode::AlphaDiscard` for better performance when pixels are fully opaque or fully transparent
- Default blend mode (no blending) for opaque sprites
- Alpha blending is expensive; avoid when not needed

### Performance Considerations

- Software rasterization is CPU-bound
- AlphaDiscard is ~50% faster than AlphaBlend
- Avoid rendering off-screen geometry (use viewport clipping)
- Power-of-2 texture dimensions enable faster address mode calculations
- Use wireframe fill mode for debugging without fill cost
