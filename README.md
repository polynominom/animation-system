# animation-system

[WIP] A from scratch animation system that uses apple's metal framework.

Required: macOS 13 or above.

Running 3d character animation example

```terminal
mkdir build
cd build
cmake .. # or cmake .. -G "Xcode" if you want to create a xcode project.
cmake --build .
./examples/anim_sys_demo/anim_sys_example
```

Output should be looking like this:

https://user-images.githubusercontent.com/16775368/212504713-243d9278-e5ce-4a80-bc66-2320ef51af84.mov


Running example for simple manbroot animation with instancing:

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



https://user-images.githubusercontent.com/16775368/212504938-f463b12b-bab6-47fa-bf31-be8d44f156fb.mov



Core Tasks:
- [x] Implement Skeleton Mesh and Animation parsing.
- [ ] Add skinnin joint bone information into rendering.
- [ ] Implement Generic BlendTree that can handle 1D and 2D lerp blending operations as well as additive blending.
- [ ] Add InverseKinematic Solvers for LookAt, Foot and GrabReach.
- [ ] Entity Component System.
- [ ] CrossFade transition mechanism with blend trees.
- [ ] AnimationStateMachine that treats every blend tree as a different state.
- [ ] Integrate hierarchy into AnimationStateMachine and make it layered.
- [ ] Allow ragdoll physics to effect to Post Processing stage in the pipeline.
