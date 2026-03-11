# Building ZIM Core

This guide provides detailed instructions for building ZIM Core applications on different platforms and with different compilers.

## Table of Contents

- [Requirements](#requirements)
- [Quick Start](#quick-start)
- [Windows](#windows)
- [Linux](#linux)
- [macOS](#macos)
- [CMake Build](#cmake-build)
- [IDE Setup](#ide-setup)
- [Troubleshooting](#troubleshooting)

---

## Requirements

### Minimum Requirements

- **C++ Compiler**: C++11 support or higher
- **Standard Library**: Full C++ standard library
- **Operating System**: 
  - Windows 7 or higher (currently supported)
  - Linux (planned)
  - macOS (planned)

### Recommended

- **C++ Standard**: C++17 or higher
- **Compiler**: Latest stable version
- **Terminal**: Modern terminal with color support

---

## Quick Start

### Basic Compilation

The simplest way to build a ZIM application:

```bash
# Windows (MSVC)
cl /EHsc /std:c++17 your_app.cpp zim\*.cpp /Fe:your_app.exe

# Windows/Linux (GCC)
g++ -std=c++17 your_app.cpp zim/*.cpp -o your_app

# macOS (Clang)
clang++ -std=c++17 your_app.cpp zim/*.cpp -o your_app
```

---

## Windows

### Using Microsoft Visual C++ (MSVC)

#### Command Line

1. Open **Developer Command Prompt for VS**

2. Navigate to your project directory:
   ```cmd
   cd C:\path\to\your\project
   ```

3. Compile:
   ```cmd
   cl /EHsc /std:c++17 main.cpp zim\Application.cpp zim\Window_win32.cpp zim\Panel.cpp zim\Input.cpp zim\Types.cpp /Fe:myapp.exe
   ```

4. Run:
   ```cmd
   myapp.exe
   ```

#### Build Options

```cmd
# Debug build with symbols
cl /EHsc /Zi /std:c++17 main.cpp zim\*.cpp /Fe:myapp.exe

# Release build with optimizations
cl /EHsc /O2 /std:c++17 main.cpp zim\*.cpp /Fe:myapp.exe

# With warnings as errors
cl /EHsc /WX /W4 /std:c++17 main.cpp zim\*.cpp /Fe:myapp.exe
```

### Using MinGW-w64 (GCC on Windows)

1. Install MinGW-w64
2. Add to PATH
3. Compile:
   ```bash
   g++ -std=c++17 -Wall main.cpp zim/*.cpp -o myapp.exe
   ```

### Using Clang on Windows

```bash
clang++ -std=c++17 -Wall main.cpp zim/*.cpp -o myapp.exe
```

---

## Linux

> **Note**: Linux support is planned but not yet implemented. These instructions are for future use.

### Using GCC

```bash
# Install GCC (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install build-essential

# Compile
g++ -std=c++17 -Wall -Wextra main.cpp zim/*.cpp -o myapp

# Run
./myapp
```

### Using Clang

```bash
# Install Clang (Ubuntu/Debian)
sudo apt-get install clang

# Compile
clang++ -std=c++17 -Wall -Wextra main.cpp zim/*.cpp -o myapp

# Run
./myapp
```

### Build Options

```bash
# Debug build
g++ -std=c++17 -g -Wall main.cpp zim/*.cpp -o myapp

# Release build with optimizations
g++ -std=c++17 -O3 -Wall main.cpp zim/*.cpp -o myapp

# With threading support (if needed in future)
g++ -std=c++17 -pthread main.cpp zim/*.cpp -o myapp
```

---

## macOS

> **Note**: macOS support is planned but not yet implemented.

### Using Clang (Xcode Command Line Tools)

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Compile
clang++ -std=c++17 -Wall main.cpp zim/*.cpp -o myapp

# Run
./myapp
```

---

## CMake Build

For larger projects, use CMake for cross-platform builds.

### CMakeLists.txt Example

Create `CMakeLists.txt` in your project root:

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyZIMApp VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ZIM Core source files
set(ZIM_SOURCES
    zim/Application.cpp
    zim/Window_win32.cpp
    zim/Panel.cpp
    zim/Input.cpp
    zim/Types.cpp
)

# Your application source
set(APP_SOURCES
    main.cpp
)

# Create executable
add_executable(myapp ${APP_SOURCES} ${ZIM_SOURCES})

# Include directories
target_include_directories(myapp PRIVATE ${CMAKE_SOURCE_DIR})

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    target_compile_definitions(myapp PRIVATE UNICODE _UNICODE)
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    # (add when Linux support is implemented)
elseif(APPLE)
    # macOS-specific settings
    # (add when macOS support is implemented)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(myapp PRIVATE /W4)
else()
    target_compile_options(myapp PRIVATE -Wall -Wextra -Wpedantic)
endif()
```

### Building with CMake

```bash
# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build
cmake --build .

# Or use platform-specific commands
# Windows (MSVC)
msbuild myapp.sln

# Linux/macOS (Make)
make
```

### CMake Build Types

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .

# With verbose output
cmake --build . --verbose
```

---

## IDE Setup

### Visual Studio

1. Create **New Project** → **Empty Project**
2. Add to **Source Files**:
   - `main.cpp`
   - All `.cpp` files from `zim/` folder
3. Add to **Header Files**:
   - All `.hpp` files from `zim/` folder
4. Set **C++ Language Standard** to C++17:
   - Project Properties → C/C++ → Language → C++ Language Standard
5. Build and run

### Visual Studio Code

Create `.vscode/tasks.json`:

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build ZIM App",
            "type": "shell",
            "command": "g++",
            "args": [
                "-std=c++17",
                "-Wall",
                "main.cpp",
                "zim/*.cpp",
                "-o",
                "myapp"
            ],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        }
    ]
}
```

Create `.vscode/launch.json`:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug ZIM App",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/myapp",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ]
        }
    ]
}
```

### CLion

1. Open project folder
2. CLion will detect CMakeLists.txt (if present)
3. Or create a new CMake project and add files
4. Configure CMake settings
5. Build and run

### Code::Blocks

1. Create **New Project** → **Console Application**
2. Add all ZIM source and header files
3. Set **Compiler** → **Compiler Settings** → **C++ Standard** to C++17
4. Build and run

---

## Makefile Build

Create a `Makefile`:

```makefile
# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Source files
ZIM_SOURCES = zim/Application.cpp zim/Window_win32.cpp zim/Panel.cpp zim/Input.cpp zim/Types.cpp
APP_SOURCES = main.cpp
SOURCES = $(APP_SOURCES) $(ZIM_SOURCES)

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
TARGET = myapp

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rebuild
rebuild: clean all

.PHONY: all clean rebuild
```

Usage:
```bash
# Build
make

# Clean
make clean

# Rebuild
make rebuild
```

---

## Troubleshooting

### Common Issues

#### Missing Header Files

**Error**: `Cannot open include file: 'zim.hpp'`

**Solution**: Ensure the `zim/` folder is in your project directory and include path is correct:
```cpp
#include "zim/zim.hpp"  // Correct
// Not: #include "zim.hpp"
```

#### Linker Errors

**Error**: `unresolved external symbol`

**Solution**: Make sure all `.cpp` files from `zim/` are included in compilation:
```bash
# Include all source files
g++ main.cpp zim/Application.cpp zim/Window_win32.cpp zim/Panel.cpp zim/Input.cpp zim/Types.cpp -o myapp
```

#### C++ Standard Not Supported

**Error**: `'auto' may only be used with static storage duration`

**Solution**: Update C++ standard to C++11 or higher:
```bash
g++ -std=c++17 main.cpp zim/*.cpp -o myapp
```

#### Windows-Specific Errors

**Error**: `GetConsoleScreenBufferInfo undefined`

**Solution**: Using wrong Window implementation. On Windows, ensure `Window_win32.cpp` is used.

### Platform-Specific Notes

#### Windows
- Use `Window_win32.cpp` for Windows builds
- Ensure Windows SDK is installed for MSVC
- May need to link against console libraries (usually automatic)

#### Terminal Requirements
- Terminal must support ANSI colors (most modern terminals do)
- Windows: Use Windows Terminal or PowerShell for best results
- Avoid old CMD.exe for color support

### Performance Tips

```bash
# Enable optimizations
g++ -O3 -std=c++17 main.cpp zim/*.cpp -o myapp

# Profile-guided optimization (advanced)
g++ -O3 -fprofile-generate main.cpp zim/*.cpp -o myapp
./myapp  # Run to collect profile data
g++ -O3 -fprofile-use main.cpp zim/*.cpp -o myapp
```

### Debug Builds

```bash
# GCC/Clang with debug symbols
g++ -g -std=c++17 main.cpp zim/*.cpp -o myapp_debug

# MSVC with debug symbols
cl /Zi /EHsc /std:c++17 main.cpp zim\*.cpp /Fe:myapp_debug.exe

# Run with debugger
gdb ./myapp_debug  # Linux/macOS
```

---

## Next Steps

After successfully building:
1. Read [Getting Started Guide](docs/GETTING_STARTED.md)
2. Explore [Examples](docs/EXAMPLES.md)
3. Check [API Reference](docs/API_REFERENCE.md)

---

## Getting Help

- **Build Issues**: Check [GitHub Issues](https://github.com/YOUR_USERNAME/ZIM_core/issues)
- **Questions**: Use [GitHub Discussions](https://github.com/YOUR_USERNAME/ZIM_core/discussions)
- **Contributing**: See [CONTRIBUTING.md](CONTRIBUTING.md)

---

**Last Updated**: March 11, 2026
