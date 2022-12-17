# animation-system

[WIP] A from scracth animation system that uses apple's metal framework.

Required: macOS 13 or above.

```terminal
mkdir build
cd build
cmake ..
cmake --build
./example/metal_demo/metal_example
```

Example Tasks:

- [x] Add External libs: glfw, imgui, metal ...
- [x] Basic Shader usage
- [x] Basic transformation
- [ ] Basic camera
- [ ] Basic lighting
- [ ] Basic texture
- [ ] Basic scene
- [ ] Basic user input handling

Core Tasks:

- [ ] Entity Component System
- [ ] Implement simple 1D BlendTree
- [ ] Extend 2D BlendTree
- [ ] Implement Skeleton Mesh and Animation parsing
- [ ] Implement InverseKinematic
