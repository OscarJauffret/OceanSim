# OceanSim

Real-time ocean surface simulator written in C++20 and OpenGL 4.1. Waves are generated on the CPU from the JONSWAP directional spectrum, then displaced and shaded on the GPU using Gerstner wave superposition.

## Features

- **Physically-motivated wave spectrum.** Wave amplitudes and frequencies sampled from the JONSWAP spectrum using wind speed, fetch, and direction from `config.json`.
- **Gerstner wave displacement.** Up to 900 wave components.
- **Water lighting.** Fresnel reflection/refraction split, tight sun specular glint, fake subsurface scattering on backlit crests, deep/shallow color blending, Reinhard tone mapping.
- **Foam from the surface Jacobian.** Near-breaking regions where the Gerstner map compresses (J → 0) are shaded as foam.
- **Fly-through camera.**

## Build

Requires CMake ≥ 3.30, a C++20 compiler, and the following libraries (install via Homebrew on macOS):

```bash
brew install glfw glew glm
```

Then:

```bash
mkdir build && cd build
cmake ..
make
```

## Run

Run from the project root so the relative shader paths in `config.json` resolve correctly:

```bash
./build/OceanSim
```

## Controls

| Key | Action |
|-----|--------|
| **W / A / S / D** | Move forward / left / back / right |
| **Space / Shift** | Move up / down |
| **Mouse** | Look around |
| **Esc** | Release cursor / close window |

## Configuration

All runtime parameters live in `config.json`:

| Section | Key | Meaning |
|---------|-----|---------|
| `window` | `width`, `height`, `title` | Window size and title |
| `shaders` | `vertex`, `fragment` | Shader paths, relative to the executable's working directory |
| `ocean` | `resolution` | Grid vertices per side (so `(resolution+1)²` vertices total) |
| `ocean` | `size` | Physical size of the ocean patch in world units |
| `ocean` | `wireframe` | Render as wireframe instead of filled triangles |
| `ocean` | `numberOfWaves` | Number of Gerstner wave components to generate and upload |
| `physics` | `gravity` | Gravitational acceleration used in the dispersion relation |
| `physics` | `windDirection` | Mean wind direction in radians |
| `physics` | `windSpeed10m` | Wind speed at 10 m altitude (used for JONSWAP α and ωₚ) |
| `physics` | `windSpeed195m` | Wind speed at 19.5 m altitude (used for the spectrum normalization) |
| `physics` | `fetch` | Distance over which the wind has blown (affects peak frequency) |
| `physics` | `windSpread` | Angular spread of wave directions around `windDirection` |

Source layout:

- `src/main.cpp` — entry point, main loop, input, FPS counter.
- `src/oceanManager.cpp` — JONSWAP spectrum sampling, wave generation.
- `src/mesh.cpp` — grid mesh generation and VAO/VBO management.
- `src/shader.cpp` — shader compile/link and uniform upload helpers.
- `src/window.cpp` — GLFW window and context wrapper.
- `include/camera.hpp` — header-only fly-camera.
- `include/oceanConfig.hpp` — configuration and `Wave` structs.
- `shaders/base.vert` — Gerstner displacement, analytic tangents/binormals, Jacobian.
- `shaders/base.frag` — water lighting model and foam.

## References

The `docs/` folder contains background material on ocean wave modeling:

- *Dynamic ocean inverse modeling based on differentiable simulation*
- *Teaching article on waves in a flume*
- *Learn OpenGL — Hello Triangle*

Further reading:

- Tessendorf, J. *Simulating Ocean Water* — the canonical reference for JONSWAP + iFFT ocean synthesis.
- Hasselmann et al. (1973) — original JONSWAP paper.
