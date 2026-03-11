# ZIM Core 🖥️

A lightweight, cross-platform C++ framework for building terminal-based user interfaces with modern, declarative patterns.

## Overview

ZIM Core is a powerful C++ library that enables developers to create rich, interactive console applications with a familiar, DOM-like element structure. It provides an event-driven architecture for building text-based UIs with support for layouts, colors, styling, and user input handling.

### Key Features

- **Element-Based Architecture**: Build UIs using a hierarchical element structure similar to web DOM
- **Event System**: Robust event handling with custom event listeners
- **Layout Management**: Automatic layout computation with flexible positioning
- **Panel System**: Create bordered, colored panels for organizing content
- **Window Management**: Advanced terminal window control with buffer management
- **Rich Styling**: Support for colors, text styles (bold, italic, underline, etc.)
- **Input Handling**: Comprehensive keyboard and mouse event support
- **Cross-Platform Ready**: Designed with Windows support and extensible for other platforms

## Quick Start

### Prerequisites

- C++ compiler with C++11 support or higher
- Windows OS (current implementation)
- Standard C++ library

### Basic Example

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    // Create application
    Application app;
    
    // Create UI element
    Element ui;
    ui.Position() = MakeVector2D(10, 10);
    ui.Dimension() = MakeVector2D(12, 12);
    
    // Add to document
    app.GetDocument().AppendChildren(ui);
    
    // Run application
    return app.Execute();
}
```

## Project Structure

```
ZIM_core/
├── zim/                    # Core library source
│   ├── zim.hpp            # Main header (includes all components)
│   ├── Application.hpp/cpp # Application and Element classes
│   ├── Window.hpp/cpp     # Window management
│   ├── Panel.hpp/cpp      # Panel components
│   ├── Input.hpp/cpp      # Input handling
│   └── Types.hpp/cpp      # Common types and utilities
├── tests/                 # Test files
│   └── testui.cpp        # Example test application
└── docs/                  # Documentation
```

## Core Components

### Application
The main entry point for ZIM applications. Manages the application lifecycle, window, and root document element.

### Element
Building blocks of your UI. Elements can contain child elements, handle events, and manage their own layout and rendering.

### Window
Manages the terminal window, including buffer operations, rendering, cursor control, and screen refreshes.

### Panel
A rectangular UI component with optional borders, titles, and customizable colors.

### Input & Events
Handles user input events (keyboard, mouse) and distributes them to registered event listeners.

## Building Your Application

### Compilation

1. Include the ZIM headers in your project
2. Link all `.cpp` files from the `zim/` directory
3. Compile with C++11 or higher

Example compilation command:
```bash
g++ -std=c++11 your_app.cpp zim/*.cpp -o your_app
```

## Documentation

- [Getting Started Guide](docs/GETTING_STARTED.md) - Detailed tutorial for beginners
- [API Reference](docs/API_REFERENCE.md) - Complete API documentation
- [Contributing Guidelines](CONTRIBUTING.md) - How to contribute to the project
- [Code of Conduct](CODE_OF_CONDUCT.md) - Community guidelines

## Use Cases

- **Terminal Applications**: Build rich TUI applications
- **System Utilities**: Create interactive system management tools
- **Games**: Develop text-based games with event handling
- **Dashboards**: Create monitoring and data visualization interfaces
- **Development Tools**: Build IDE-like tools in the terminal

## Roadmap

- [ ] Linux and macOS support
- [ ] More layout algorithms (flexbox-like, grid)
- [ ] Enhanced styling options
- [ ] Animation support
- [ ] More input device support
- [ ] Theme system
- [ ] Built-in widgets library

## Contributing

We welcome contributions! Please read our [Contributing Guidelines](CONTRIBUTING.md) for details on:
- Code style and standards
- How to submit pull requests
- Reporting bugs and requesting features
- Development workflow

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Community & Support

- **Issues**: Report bugs or request features on [GitHub Issues](../../issues)
- **Discussions**: Join conversations in [GitHub Discussions](../../discussions)
- **Pull Requests**: Contribute code via [Pull Requests](../../pulls)

## Authors & Acknowledgments

Developed and maintained by the ZIM Core team and contributors.

---

**Note**: ZIM Core is actively under development. APIs may change between versions. Check the changelog for updates.
