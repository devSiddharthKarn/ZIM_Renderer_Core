# Getting Started with ZIM Core

This guide will walk you through creating your first ZIM Core application from scratch.

## Table of Contents

- [Installation](#installation)
- [Your First Application](#your-first-application)
- [Understanding the Basics](#understanding-the-basics)
- [Working with Elements](#working-with-elements)
- [Handling Events](#handling-events)
- [Customizing Appearance](#customizing-appearance)
- [Advanced Layouts](#advanced-layouts)
- [Best Practices](#best-practices)

## Installation

### Step 1: Clone the Repository

```bash
git clone https://github.com/YOUR_USERNAME/ZIM_core.git
cd ZIM_core
```

### Step 2: Verify Your Compiler

Ensure you have a C++11 compatible compiler:

```bash
# For GCC
g++ --version

# For MSVC (Windows)
cl

# For Clang
clang++ --version
```

### Step 3: Project Structure

```
your_project/
├── zim/                 # Copy the entire zim folder here
│   ├── zim.hpp
│   ├── Application.hpp/cpp
│   ├── Window.hpp/cpp
│   └── ...
└── main.cpp            # Your application code
```

## Your First Application

### Hello ZIM

Create a file called `main.cpp`:

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    // Create the application
    Application app;
    
    // Get the root document element
    Element& document = app.GetDocument();
    
    // Create a UI element
    Element ui;
    ui.Position() = MakeVector2D(5, 5);
    ui.Dimension() = MakeVector2D(20, 10);
    
    // Add element to the document
    document.AppendChildren(ui);
    
    // Run the application (event loop)
    return app.Execute();
}
```

### Compile and Run

```bash
# Windows (MSVC)
cl /EHsc main.cpp zim/*.cpp /Fe:myapp.exe
myapp.exe

# Windows/Linux (GCC)
g++ -std=c++11 main.cpp zim/*.cpp -o myapp
./myapp

# macOS (Clang)
clang++ -std=c++11 main.cpp zim/*.cpp -o myapp
./myapp
```

## Understanding the Basics

### Application Class

The `Application` class is the heart of your ZIM application:

```cpp
Application app;           // Create application
Window& win = app.GetWindow();        // Access window
Element& doc = app.GetDocument();     // Access root element
int result = app.Execute();           // Run event loop
```

### Element Hierarchy

Elements can contain child elements, creating a tree structure:

```cpp
Element parent;
Element child1, child2;

// Add children to parent
parent.AppendChildren(child1);
parent.AppendChildren(child2);
```

### Coordinate System

ZIM uses a 2D coordinate system:
- Origin (0,0) is typically the top-left corner
- X increases to the right
- Y increases downward

```cpp
Vector2D pos = MakeVector2D(10, 5);  // x=10, y=5
element.Position() = pos;
```

## Working with Elements

### Creating Elements

```cpp
Element myElement;

// Set position (x, y)
myElement.Position() = MakeVector2D(10, 10);

// Set size (width, height)
myElement.Dimension() = MakeVector2D(30, 15);

// Activate the element
myElement.Active() = Logic::True;
```

### Element IDs

Assign IDs to elements for easy retrieval:

```cpp
Element myElement;
myElement.Id() = "myButton";

// Later, find by ID
Element* found = document.FindElementById("myButton");
if (found != nullptr) {
    // Use the element
}
```

### Panels

Elements contain panels for rendering:

```cpp
Element element;
Panel& panel = element.GetPanel();

// Set panel properties
panel.Title().text = "My Panel";
panel.Title().visible = Logic::True;
panel.SetBufferBGColor(MakeColor(0, 0, 128));  // Blue background
panel.SetBufferFGColor(MakeColor(255, 255, 255));  // White text
```

## Handling Events

### Event Listeners

Attach event listeners to elements to respond to user input:

```cpp
element.DefineEventListener([](EventImage& event, Element& self, Element& doc) {
    // Handle mouse click
    if (event.mouseInput.leftButton == Logic::True) {
        // Do something when clicked
    }
    
    // Handle keyboard input
    if (event.keyboardInput.keyPressed == Logic::True) {
        // Handle key press
    }
});
```

### Event Types

ZIM supports various event types:

```cpp
void MyEventHandler(EventImage& event, Element& self, Element& doc) {
    // Mouse events
    if (event.mouseInput.leftButton == Logic::True) {
        // Left click
    }
    
    if (event.mouseInput.rightButton == Logic::True) {
        // Right click
    }
    
    // Keyboard events
    if (event.keyboardInput.keyPressed == Logic::True) {
        char key = event.keyboardInput.keyChar;
        // Process key
    }
}

element.DefineEventListener(MyEventHandler);
```

## Customizing Appearance

### Colors

ZIM supports RGB colors:

```cpp
Color red = MakeColor(255, 0, 0);
Color green = MakeColor(0, 255, 0);
Color blue = MakeColor(0, 0, 255);
Color white = MakeColor(255, 255, 255);
Color black = MakeColor(0, 0, 0);

Panel& panel = element.GetPanel();
panel.SetBufferBGColor(blue);   // Background color
panel.SetBufferFGColor(white);  // Foreground (text) color
```

### Text Styles

Apply various text styles:

```cpp
OverloadStyle style = OverloadStyle::Bold | OverloadStyle::Underline;

// Use in PointDetail when setting individual characters
PointDetail detail;
detail.style = style;
detail.fgColor = MakeColor(255, 255, 255);
detail.bgColor = MakeColor(0, 0, 0);
detail.ch = 'A';
```

### Available Styles

```cpp
OverloadStyle::Bold
OverloadStyle::Dim
OverloadStyle::Italic
OverloadStyle::Underline
OverloadStyle::BlinkSlow
OverloadStyle::BlinkFast
OverloadStyle::Reverse
OverloadStyle::Hidden
OverloadStyle::Strikethrough
OverloadStyle::Framed
OverloadStyle::Encircled
OverloadStyle::Overline
```

## Advanced Layouts

### Custom Layout Handler

Define how child elements are positioned:

```cpp
element.DefineLayoutHandler([](Element& parent, Element& self, Element& doc) {
    // Custom layout logic
    Vector2D parentDim = parent.Dimension();
    
    // Center the element
    self.Position().x = (parentDim.x - self.Dimension().x) / 2;
    self.Position().y = (parentDim.y - self.Dimension().y) / 2;
});
```

### Building a Button

```cpp
Element CreateButton(int x, int y, int width, int height, std::string text) {
    Element button;
    button.Position() = MakeVector2D(x, y);
    button.Dimension() = MakeVector2D(width, height);
    button.Active() = Logic::True;
    
    Panel& panel = button.GetPanel();
    panel.Title().text = text;
    panel.Title().visible = Logic::True;
    panel.SetBufferBGColor(MakeColor(100, 100, 100));
    panel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    button.DefineEventListener([](EventImage& e, Element& self, Element& doc) {
        if (e.mouseInput.leftButton == Logic::True) {
            // Button clicked!
            Panel& p = self.GetPanel();
            p.SetBufferBGColor(MakeColor(200, 200, 200));
        }
    });
    
    return button;
}

// Usage
Element myButton = CreateButton(10, 10, 20, 5, "Click Me");
document.AppendChildren(myButton);
```

### Building a List

```cpp
void CreateList(Element& parent, int x, int y, int itemHeight) {
    std::vector<std::string> items = {"Item 1", "Item 2", "Item 3", "Item 4"};
    
    for (size_t i = 0; i < items.size(); i++) {
        Element item;
        item.Position() = MakeVector2D(x, y + (i * itemHeight));
        item.Dimension() = MakeVector2D(30, itemHeight);
        item.Active() = Logic::True;
        
        Panel& panel = item.GetPanel();
        panel.Title().text = items[i];
        panel.Title().visible = Logic::True;
        
        parent.AppendChildren(item);
    }
}
```

## Best Practices

### Memory Management

```cpp
// Elements are typically created on the stack
Element element;

// Or dynamically if needed
Element* dynamicElement = new Element();
// Remember to delete when done
delete dynamicElement;
```

### Performance Tips

1. **Minimize redraws**: Only update elements when necessary
2. **Batch updates**: Group multiple changes together
3. **Use appropriate dimensions**: Don't make elements unnecessarily large
4. **Efficient event handlers**: Keep event handler logic simple and fast

### Debugging

```cpp
// Use window controls for debugging
Window& window = app.GetWindow();

// Check dimensions
Vector2D dims = window.GetDimensions();

// Store command buffer for analysis
window.StoreCommandBuffer("debug_output.txt", Logic::True);
```

### Error Handling

```cpp
// Check for null pointers
Element* found = document.FindElementById("myElement");
if (found == nullptr) {
    // Handle not found
    return;
}

// Validate coordinates
Vector2D pos = element.Position();
Vector2D dims = window.GetDimensions();
if (pos.x >= dims.x || pos.y >= dims.y) {
    // Out of bounds
}
```

## Complete Example: Interactive Menu

Here's a complete example that puts it all together:

```cpp
#include "zim/zim.hpp"
#include <vector>
#include <string>

int main() {
    using namespace zim;
    
    Application app;
    Element& document = app.GetDocument();
    
    // Create menu container
    Element menu;
    menu.Position() = MakeVector2D(10, 5);
    menu.Dimension() = MakeVector2D(40, 20);
    menu.Active() = Logic::True;
    menu.Id() = "menu";
    
    Panel& menuPanel = menu.GetPanel();
    menuPanel.Title().text = "Main Menu";
    menuPanel.Title().visible = Logic::True;
    menuPanel.SetBufferBGColor(MakeColor(20, 20, 60));
    menuPanel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Create menu items
    std::vector<std::string> items = {"New Game", "Load Game", "Settings", "Exit"};
    
    for (size_t i = 0; i < items.size(); i++) {
        Element item;
        item.Position() = MakeVector2D(2, 2 + (i * 3));
        item.Dimension() = MakeVector2D(36, 2);
        item.Active() = Logic::True;
        
        Panel& itemPanel = item.GetPanel();
        itemPanel.Title().text = items[i];
        itemPanel.Title().visible = Logic::True;
        itemPanel.SetBufferBGColor(MakeColor(40, 40, 80));
        itemPanel.SetBufferFGColor(MakeColor(200, 200, 200));
        
        // Add hover effect
        item.DefineEventListener([](EventImage& e, Element& self, Element& doc) {
            Panel& p = self.GetPanel();
            
            if (e.mouseInput.leftButton == Logic::True) {
                // Clicked
                p.SetBufferBGColor(MakeColor(100, 100, 200));
            } else {
                // Hover (simplified - check if mouse is over element)
                p.SetBufferBGColor(MakeColor(60, 60, 120));
            }
        });
        
        menu.AppendChildren(item);
    }
    
    document.AppendChildren(menu);
    
    return app.Execute();
}
```

## Next Steps

- Explore the [API Reference](API_REFERENCE.md) for detailed documentation
- Check out more examples in the `tests/` directory
- Read [CONTRIBUTING.md](../CONTRIBUTING.md) to contribute to the project
- Join the community to share your creations!

## Troubleshooting

### Common Issues

**Issue**: Elements don't appear
- Check that `Active()` is set to `Logic::True`
- Verify position is within window bounds
- Ensure element has non-zero dimensions

**Issue**: Events not firing
- Make sure event listener is defined before `Execute()`
- Check that element is active
- Verify element dimensions cover the click area

**Issue**: Colors not working
- Ensure terminal supports colors (most modern terminals do)
- Check RGB values are in range 0-255

## Getting Help

- **Documentation**: Read the full [API Reference](API_REFERENCE.md)
- **Issues**: Check [GitHub Issues](../../issues) for known problems
- **Discussions**: Ask questions in [GitHub Discussions](../../discussions)
- **Examples**: Study code in the `tests/` directory

Happy coding with ZIM Core!
