\# RetroRay Engine



RetroRay is a high-performance, pseudo-3D raycasting game engine built entirely from scratch in C++ using \*\*Raylib 5.5\*\* and the modern \*\*GCC 15.2.0 (UCRT64)\*\* toolchain. 



Inspired by classic 90s titles like \*Wolfenstein 3D\* and \*Doom\*, this engine handles real-time mathematical perspective projections, spatial partition grid array lookups, and custom axis-separated sliding collision physics to create a fully immersive 3D simulation out of a flat 2D matrix layout.



\---



\## Technical Architecture \& Core Features



\* \*\*Real-Time Mathematical Projection:\*\* Casts dynamic rays across a 60° Field of View (FOV) matching the horizontal pixel width of the window context. 

\* \*\*Fish-Eye Distortion Correction:\*\* Eliminates the classic curved-wall rendering bug by computing linear depth vectors via the cosine dot-product shortcut relative to the player's central viewpoint angle.

\* \*\*16x16 Procedural Maze Database:\*\* Features structured choke points, isolated pillars, dead ends, and long sightline corridors designed to test dynamic rendering thresholds.

\* \*\*Axis-Separated Collision Sliding:\*\* Evaluates independent horizontal ($X$) and vertical ($Y$) tracking steps against solid boundaries. This prevents diagonal movement freezes, allowing the player to seamlessly slide along wall surfaces.

\* \*\*Atmospheric Distance Shading:\*\* Implements linear lighting drop-off interpolation based on calculated ray distance vectors to mimic depth and atmospheric fog.

\* \*\*HUD Minimap Overlay:\*\* Renders a translucent, scaled down 2D map widget in the top-left corner displaying real-time tracking of player positions and sightline vectors.



\---



\## Installation \& Prerequisites



To compile and execute the engine, ensure your development machine is equipped with a C++11 (or higher) compiler and the Raylib development libraries.



\### Windows (via MSYS2)

The universal runtime (`UCRT64`) environment is recommended for Windows development.

1\. Download and install \*\*MSYS2\*\* from \[msys2.org](https://www.msys2.org/).

2\. Launch the \*\*MSYS2 UCRT64\*\* terminal and execute the following command to update packages and force-install the GCC toolchain alongside Raylib:

&#x20;  ```bash

&#x20;  pacman -S --needed --noconfirm mingw-w64-ucrt-x86\_64-toolchain mingw-w64-ucrt-x86\_64-raylib

