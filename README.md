# A* Pathfinding Algorithm Demo

An interactive visualization of the A* (A-Star) pathfinding algorithm, built with C++ and native Win32 API. This project demonstrates real-time pathfinding on a customizable grid where you can place obstacles and adjust start/end points.

![Platform](https://img.shields.io/badge/platform-Windows-blue)
![Language](https://img.shields.io/badge/language-C%2B%2B-orange)
![License](https://img.shields.io/badge/license-MIT-green)

## Features

- **Interactive Grid**: 32×18 tile grid for pathfinding visualization
- **Real-time Path Calculation**: Instantly updates the path when obstacles or endpoints change
- **Custom Obstacle Placement**: Click to toggle obstacles on any tile
- **Adjustable Start/End Points**: Set custom start and finish positions
- **Visual Feedback**: Color-coded display showing the algorithm's progress

### Color Legend

| Color | Meaning |
|-------|---------|
| 🟢 Green | Start node |
| 🔴 Red | End/Goal node |
| 🟡 Yellow | Calculated shortest path |
| 🔵 Blue | Visited nodes (explored during search) |
| ⬜ Grey | Obstacles |
| 🔷 Dark Blue | Unvisited traversable tiles |

## Controls

| Action | Control |
|--------|---------|
| Toggle obstacle | **Left Click** on a tile |
| Set start position | **Shift + Left Click** on a tile |
| Set end position | **Ctrl + Left Click** on a tile |
| Close application | **Alt + F4** |

## Prerequisites

- **Operating System**: Windows (x64)
- **Compiler**: Visual Studio 2022 with C++ desktop development workload
- **Build System**: CMake 3.10 or higher

## Building the Project

### Using CMake Presets (Recommended)

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/a_start_demo.git
   cd a_start_demo
   ```

2. Configure the project:
   ```bash
   cmake --preset msvc-configure
   ```

3. Build the project:
   ```bash
   # Debug build
   cmake --build --preset msvc-build-debug

   # Release build
   cmake --build --preset msvc-build-release
   ```

4. The executable will be located at:
   ```
   out/build/msvc-configure/Debug/astar_demo.exe   # Debug
   out/build/msvc-configure/Release/astar_demo.exe # Release
   ```

### Using Visual Studio

1. Open the project folder in Visual Studio 2022
2. Visual Studio should automatically detect the CMake configuration
3. Select your desired build configuration (Debug/Release)
4. Build and run with F5

## Project Structure

```
a_start_demo/
├── include/
│   ├── astar.h           # A* algorithm interface
│   ├── demo.h            # Demo application interface
│   └── win32_platform.h  # Win32 platform layer definitions
├── src/
│   ├── astar.cpp         # A* algorithm implementation
│   ├── demo.cpp          # Grid rendering and interaction logic
│   └── win32_platform.cpp # Win32 window and input handling
├── CMakeLists.txt        # CMake build configuration
├── CMakePresets.json     # CMake preset configurations
├── LICENSE               # MIT License
└── README.md
```

## Technical Details

### A* Algorithm Implementation

The A* algorithm finds the shortest path between two nodes using a combination of:

- **g(n)**: The actual cost from the start node to node n (`localGoal`)
- **h(n)**: The heuristic estimate from node n to the goal (`distance` function)
- **f(n)**: The total estimated cost: f(n) = g(n) + h(n) (`globalGoal`)

Key implementation details:
- Uses **Euclidean distance** as the heuristic function
- Maintains an open list of nodes to explore, sorted by `globalGoal`
- Supports **4-directional movement** (up, down, left, right)
- Efficiently backtracks using parent pointers to reconstruct the path

### Rendering Architecture

The demo uses a **software rendering** approach with Win32 GDI:

1. **Offscreen Buffer**: A DIB (Device Independent Bitmap) is used for double-buffered rendering
2. **Direct Pixel Manipulation**: Colors are written directly to memory for fast rendering
3. **StretchDIBits**: The buffer is copied to the window using this Win32 function

### Build Configuration

The project uses specific MSVC compiler options for development:
- `/W4` - High warning level
- `/WX` - Treat warnings as errors
- `/Z7` - Debug information format
- `/MTd` - Static C runtime library (debug)

## How It Works

1. **Initialization**: A 32×18 grid of nodes is created, each connected to its 4 neighbors
2. **User Interaction**: Mouse clicks modify the grid (obstacles, start, end positions)
3. **Path Calculation**: When the grid changes, A* is invoked to find the optimal path
4. **Rendering**: The grid is redrawn each frame, visualizing nodes and the calculated path

## Algorithm Pseudocode

```
1. Initialize all nodes with infinite costs
2. Set start node's local cost to 0, global cost to h(start)
3. Add start node to open list
4. While open list is not empty and current ≠ goal:
   a. Sort open list by global cost (ascending)
   b. Remove visited nodes from front of list
   c. Current = first node in open list
   d. Mark current as visited
   e. For each unvisited, non-obstacle neighbor:
      - Add to open list if not present
      - Calculate tentative local cost
      - If better than existing, update parent and costs
5. Backtrack from goal using parent pointers to get path
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- Inspired by various A* visualization tutorials and demos
- Built using techniques from the Win32 API documentation
