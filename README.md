# C++ Ray Tracer

A 3D ray tracer built from scratch in C++. Reads a scene definition from a text file and renders it to a PPM image.

## Features
- Configurable spheres with position, radius, and color
- Configurable camera position and resolution
- Depth-correct occlusion via ray-sphere intersection (quadratic discriminant)
- Sky color and scene parsing from text file

## Files
- `Assignment2.cpp` — Sphere, Camera, and World classes + render loop
- `vec3.h`, `ray.h`, `color.h` — Vector math and ray utilities (Peter Shirley / Dr. Papagiannakis)
- `std_lib_facilities.h` — Standard library utilities (Bjarne Stroustrup)

## Build
```
cmake .
make
./raytracer.cpp
```

## Usage
The program reads `world.txt` for the scene definition in the following format:
```
output_name
camera_x camera_y camera_z
width
height
sky_r sky_g sky_b
sphere_count
color_r color_g color_b center_x center_y center_z radius
```

## Sample Output
![render](world.ppm)
