# Cube

Voxel rendering engine 

### Features

- [ ] World generation
  - [ ] Chunk manager system
  - [ ] Dynamic loading
  - [ ] Multithreaded generation
  - [ ] Generator 
    - [ ] Multithread support
    - [ ] Biomes support
      - [ ] Based on temperature and humidity
      - [ ] Limited height
    - [ ] Structures
- [ ] 2D Rendering
  - [X] Core rendering
    - [X] Filled shapes
    - [X] Stroked shapes
    - [X] Images
    - [X] Text
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
- [ ] 3D Rendering
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
  - [ ] Camera system
    - [ ] First person camera
    - [ ] Third person camera
  - [ ] Lighting
    - [ ] Ambient light
    - [ ] Directional light
    - [ ] Spot light
- [x] Window
  - [x] Resizeable
  - [x] Load OpenGL functions
  - [x] Fill rendering

### Dependencies

 - OpenGL 4.6 Core profile
 - GLAD ([included](/libs/glad))
 - GLM
 - GLFW 3.x
 - Freetype 2.x
 - FastNoise2 ([included as submodule](/libs/FastNoise2))
 - STB ([included](/libs/stb))
   - stb_image
   - stb_image_write

### Thanks to

 Jetbrains - for free licence for CLion

## Author

Maks Makuta (C) 2025  
MIT License