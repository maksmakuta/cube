# Cube

Voxel rendering engine 

### Features
- [ ] Engine
  - [ ] World generation
    - [ ] Chunk manager system
    - [ ] Dynamic loading
    - [ ] Multithreaded generation
      - [ ] Generator 
        - [ ] Multithread support
        - [ ] Biomes support
      - [ ] Noise implementation
        - [ ] Perlin
        - [ ] Simplex
        - [ ] Value
  - [ ] Rendering
    - [ ] 2D
      - [ ] UI Rendering
      - [ ] Layouts
        - [ ] Column
        - [ ] Row
        - [ ] Tabs
      - [ ] Widgets
        - [ ] Button
        - [ ] Label
        - [ ] Switch
        - [ ] Icon/Image
        - [ ] Input
    - [ ] 3D
      - [ ] Sky
        - [ ] Sun/Moon cycle
        - [ ] Clouds
          - [ ] Flat
          - [ ] Detailed
      - [ ] Block Rendering
        - [ ] Filled
        - [ ] X-shaped
        - [ ] #-shaped
      - [ ] Support for chunk rendering
        - [ ] Multithreading mesh generator
- [x] Window
  - [x] Resizeable
  - [x] Load OpenGL functions
  - [x] Fill rendering

### Dependencies

 - OpenGL 4.6 Core profile
 - GLAD ([included](/libs/glad))
 - GLM
 - GLFW 3.x

### Thanks to

 Jetbrains - for free licence for CLion

## Author

Maks Makuta (C) 2025  
MIT License