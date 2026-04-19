# cube

A high-performance, multithreaded voxel engine built from scratch using **C++23** and **OpenGL 4.6+** for Linux. 

> [!WARNING]  
> **Status: Raw / Experimental.** This project is currently in a pre-alpha state. Most core systems are being implemented; it is not yet in a functional or "playable" state.

## 🛠 Tech Stack (Linux-First)
- **Language:** C++23 (utilizing `std::mdspan`, `std::expected`, `std::jthread`).
- **Graphics:** OpenGL 4.6 (Core Profile) with Direct State Access (DSA).
- **Parallelization:** POSIX Threads (via `std::jthread`) with manual CPU Core Affinity.
- **Windowing/Context:** SDL3
- **Math:** GLM (OpenGL Mathematics).

## Thanks to
[Aim__Boot](https://www.curseforge.com/minecraft/texture-packs/ashen-16x)  for good texture pack

## 📜 License
This project is licensed under the **MIT License**. See the [LICENSE](LICENSE.md) file for details.  
Textures is licensed under All Rights Reserved license and used only in personal purposes to demonstrate this project