# animation-system

[WIP] A from scratch animation system that uses apple's metal framework.

Required: macOS 13 or above.

Running example:

```terminal
mkdir build
cd build
cmake .. # or cmake .. -G "Xcode" if you want to create a xcode project.
cmake --build .
cd ./example/metal_demo
./metal_example
```

Output should be looking like this(it is important to run the example in `metal_demo` folder):

<img src="docs/RenderExample.gif" width="300">

GLFW and IMGUI can be used as well, another example can be run:

```terminal
# in build folder
./examples/imgui_glfw_metal_demo/ImguiGLFWMetalDemo
```

Output example should be looking like this:

![Imgui Example Video](docs/ImguiExampleWindow.mov)

Core Tasks:

- [x] Examples
- [x] Basic components (Quaternions, Skeleton, Joints, Animation Clips, etc...)
- [x] Camera and perspective
- [x] Mesh loading and rendering
- [ ] Scene
- [ ] Implement simple 1D BlendTree
- [ ] Extend 2D BlendTree
- [ ] Implement Skeleton Mesh and Animation parsing
- [ ] Implement InverseKinematic
- [ ] Entity Component System
