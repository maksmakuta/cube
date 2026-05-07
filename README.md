# cube

High performance voxel rendering engine

--- 

## Features

### Core Systems & Memory
- [X] **Chunking System**: Divide the world into $16^3$ segments for efficient management.
- [ ] **Data Compression**: Implement Run-Length Encoding (RLE) or palette-based storage for inactive chunks.
- [ ] **Threaded Task Queue**: Move world generation and mesh building to background threads to prevent frame stutters.
- [ ] **Fast Voxel Traversal**: A raycasting algorithm (like Amanatides-Woo) for block selection and placement.

### Rendering Optimizations
- [ ] **Hidden Face Culling**: Do not generate mesh data for faces obscured by adjacent solid blocks.
- [ ] **Greedy Meshing**: Combine adjacent coplanar faces into single large quads to reduce vertex count.
- [ ] **Vertex Packing**: Use bit-shifting to pack position, UV, and lighting data into a single `uint32` per vertex.
- [ ] **Frustum Culling**: Skip draw calls for chunks that are behind the camera or out of view.
- [ ] **LOD (Level of Detail)**: Render distant chunks as simplified meshes or low-resolution textures.

### World & Physics
- [ ] **Procedural Generation**: Layered Simplex or Perlin noise for heightmaps and 3D density.
- [ ] **AABB Collision**: Axis-Aligned Bounding Box checks for player-to-block interaction.
- [ ] **Infinite Terrain Logic**: Logic to dynamically load/unload chunks based on player coordinates.
- [ ] **Biomes & Caves**: Use 3D noise (Snyder/Perlin) to "carve" underground structures.

### Lighting & Visuals
- [ ] **Flood Fill Lighting**: Propagate sunlight and point lights through the voxel grid.
- [ ] **Ambient Occlusion**: Darken the corners where blocks meet for better depth perception.
- [ ] **Transparency Handling**: Separate render passes for opaque and transparent (water/glass) voxels.
- [ ] **Post-Processing**: Implement basic shaders for fog (to hide chunk loading) and bloom.

### Interactions & Persistence
- [ ] **Serialization**: Save and load modified chunks to a local database or binary files.
- [ ] **Inventory & Block Types**: A registry system to define properties (friction, light emission, hardness) for different IDs.
- [ ] **Particle System**: Simple block-break particles that inherit the color/texture of the voxel.

---

## Tech Stack

 - C++23
 - OpenGL 4.6 Core
 - [SDL3](https://github.com/libsdl-org/SDL)
 - [glm](https://github.com/g-truc/glm)
 - [libspng](https://github.com/randy408/libspng) (submodule)
 - [glad](https://github.com/maksmakuta/glad-cmake) (submodule)
 - [FastNoise2](https://github.com/Auburn/FastNoise2) (submodule)

---

# Author & License

Maks Makuta (C) 2026 | [MIT License](LICENSE.md)