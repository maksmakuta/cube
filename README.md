# cube

A high-performance, multithreaded voxel engine built from scratch using **C++23** and **OpenGL 4.6+** for Linux. 

> [!WARNING]  
> **Status: Raw / Experimental.** This project is currently in a pre-alpha state. Most core systems are being implemented; it is not yet in a functional or "playable" state.

## 🚀 The Vision
To render massive view distances (32+ chunks) with high frame rates by offloading generation and meshing to a custom thread pool and utilizing modern GPU "Indirect Rendering" techniques to minimize driver overhead.

## 🛠 Tech Stack (Linux-First)
- **Language:** C++23 (utilizing `std::mdspan`, `std::expected`, `std::jthread`).
- **Graphics:** OpenGL 4.6 (Core Profile) with Direct State Access (DSA).
- **Parallelization:** POSIX Threads (via `std::jthread`) with manual CPU Core Affinity.
- **Windowing/Context:** SDL3
- **Math:** GLM (OpenGL Mathematics).

## 📜 License
This project is licensed under the **MIT License**. See the [LICENSE](LICENSE.md) file for details.

---

### 🗺 Project Roadmap

To reach a state where you can fly through 32+ chunks smoothly, we have to build the engine in four distinct phases.

#### Phase 1: The Foundation (Data & Memory)
* [ ] **Custom Memory Allocator:** Create a pool allocator for Voxel data to prevent fragmentation during chunk loading/unloading.
* [ ] **Chunk Representation:** Implement `std::mdspan` wrappers for 32x32x32 voxel grids.
* [ ] **World Coordinates:** Create a robust system to convert between Global, Chunk, and Local coordinates.
* [ ] **C++23 Expected Wrapper:** Build the error handling layer for file I/O and GPU buffer allocation.

#### Phase 2: The Parallel Pipeline (The "Engine" Room)
* [ ] **Worker Thread Pool:** Initialize a pool of `std::jthread` based on `hardware_concurrency()`.
* [ ] **Linux Thread Pinning:** Implement a utility to set thread affinity so meshing threads don't jump cores.

* [ ] **Greedy Meshing Algorithm:** Implement the logic to merge adjacent voxel faces into optimized quads.
* [ ] **Task Queue:** A lock-free or mutex-protected queue to send "dirty" chunks to workers and "ready" meshes back to the main thread.

#### Phase 3: The Modern Renderer (GPU Heavy-Lifting)
* [ ] **Persistent Mapping:** Implement `glMapBufferRange` to allow CPU threads to write mesh data directly into GPU-visible memory.
* [ ] **Indirect Draw Buffer:** Set up a `GL_DRAW_INDIRECT_BUFFER` to store draw commands.

* [ ] **Texture Arrays:** Use `GL_TEXTURE_2D_ARRAY` to avoid texture swapping (bindless-like behavior).
* [ ] **Frustum Culling:** Implement a basic culling system on the CPU (or Compute Shader) to avoid sending invisible chunks to the GPU.

#### Phase 4: World & Interaction
* [ ] **Terrain Generation:** Multi-octave Perlin/Simplex noise implementation using SIMD.
* [ ] **Infinite Loading:** A "sliding window" system that loads chunks around the player and discards distant ones.
* [ ] **Basic Physics:** Ray-casting for block selection and a simple AABB collision system.
