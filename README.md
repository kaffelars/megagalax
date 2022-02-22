## megagalax

Simple gravity particle simulation. Simulate anything from simple solar systems to complex galaxy collisions.

A compiled version (x64 windows) in the builds folder.

Each particle has a position, velocity and mass. Then for each frame gravity is calculated (gpu compute shader) between each particle and every other particle, imparting new velocities for each particle.

No optimizations, this is just simple brute force. But it works decently, although fps will quickly dip at higher particle counts.

Be warned: This is not a scientifically accurate tool, it's just a toy! Place particles and play around with all the different settings to get neat results.

## Example pics

![galaxy collision](https://github.com/kaffelars/megagalax/blob/main/pics/system2.png?raw=true)
![peaceful galaxy](https://github.com/kaffelars/megagalax/blob/main/pics/system3.png?raw=true)
![simple system](https://github.com/kaffelars/megagalax/blob/main/pics/system1.gif?raw=true)
![calm galaxy](https://github.com/kaffelars/megagalax/blob/main/pics/system4.png?raw=true)

## Different shaders/shader parameters

![simple system](https://github.com/kaffelars/megagalax/blob/main/pics/shaders.png?raw=true)

## How to use

When you start, you can either start with an empty system, or load one of the premade systems.

All controls are found in the left side panel (only visible when simulation is paused).

You can add single particles (normal or supermassive) or add many particles using the sphere generator. Simply click one of the "add" buttons in the left side panel, then change parameters such as start position, velocity, mass, or radius/mass distribution/flatness/rotation/etc. (sphere generator). All the units are arbitrary.

Then click "Apply" to add the particles to the simulation. To start (or stop) the simulation press "p".

Also in the left panel, you can change particle shader and shader parameters, as well as simulation parameters such as gravity and "slowdown" (how much particles move for each frame).

You can also change simulation type and parameters:
* gravity - the strength of gravity
* slowdown - how far particles move for each frame
* three different simulation shaders:
  - "simple" - particles only interact though gravity - highest fps
  - "interact" - two particles that come close share some of their velocity and/or mass with each other. Higher levels of velocity/mass transfer or higher interaction distance may produce odd results (e.g. overlapping particles, mass "explosions", etc.).
  - "merge" - two particles that come close will merge into one larger particle - change merge distance to increase/reduce the chance of a merge.


For each frame, particles are downloaded from the gpu, sorted on the cpu (back to front), then uploaded again to render (not optimal). This can be disabled.

You can also enable/disable showing particle velocities before you apply them, as well as a horizontal grid.

### Hotkeys

* p - Pause/unpause simulation
* c - change between two camera modes (pivot around center, or free camera)
* w/a/s/d - move camera forward/left/backward/right
* q/e - move camera up/down
* right mouse button - to move camera
* mouse scroll - zoom in/out

## Using
* [SDL2](https://www.libsdl.org/)
* OpenGL
* [GLM](https://github.com/g-truc/glm)
* [Dear ImGui](https://github.com/ocornut/imgui)
* [pugixml](https://pugixml.org/)
