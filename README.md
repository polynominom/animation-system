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

Tasks:

- [x] Add External libs: glfw, imgui, metal ...
- [ ] Basic Shader usage
- [ ] Basic transformation
- [ ] Entity Component System
- [ ] Implement simple 1D BlendTree
- [ ] Extend 2D BlendTree
- [ ] Implement Skeleton Mesh and Animation parsing
- [ ] Implement InverseKinematic
