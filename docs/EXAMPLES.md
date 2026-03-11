# ZIM Core Examples

This document provides practical examples of using ZIM Core to build terminal-based user interfaces.

## Table of Contents

- [Basic Examples](#basic-examples)
- [Interactive Elements](#interactive-elements)
- [Layout Examples](#layout-examples)
- [Styling Examples](#styling-examples)
- [Advanced Examples](#advanced-examples)

---

## Basic Examples

### Example 1: Hello World

The simplest ZIM application - display a single element.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    
    Element ui;
    ui.Position() = MakeVector2D(10, 10);
    ui.Dimension() = MakeVector2D(30, 5);
    ui.Active() = Logic::True;
    
    Panel& panel = ui.GetPanel();
    panel.Title().text = "Hello, ZIM!";
    panel.Title().visible = Logic::True;
    
    app.GetDocument().AppendChildren(ui);
    
    return app.Execute();
}
```

---

### Example 2: Multiple Elements

Create multiple elements on screen.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    Element& doc = app.GetDocument();
    
    // Create three panels
    for (int i = 0; i < 3; i++) {
        Element box;
        box.Position() = MakeVector2D(5 + (i * 25), 5);
        box.Dimension() = MakeVector2D(20, 10);
        box.Active() = Logic::True;
        
        Panel& panel = box.GetPanel();
        panel.Title().text = "Box " + std::to_string(i + 1);
        panel.Title().visible = Logic::True;
        panel.SetBufferBGColor(MakeColor(50 * i, 100, 200 - (50 * i)));
        panel.SetBufferFGColor(MakeColor(255, 255, 255));
        
        doc.AppendChildren(box);
    }
    
    return app.Execute();
}
```

---

## Interactive Elements

### Example 3: Clickable Button

Create a button that responds to clicks.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    
    Element button;
    button.Position() = MakeVector2D(20, 10);
    button.Dimension() = MakeVector2D(25, 5);
    button.Active() = Logic::True;
    button.Id() = "clickButton";
    
    Panel& panel = button.GetPanel();
    panel.Title().text = "Click Me!";
    panel.Title().visible = Logic::True;
    panel.SetBufferBGColor(MakeColor(70, 130, 180));
    panel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Track click count
    int clickCount = 0;
    
    button.DefineEventListener([&clickCount](EventImage& e, Element& self, Element& doc) {
        if (e.mouseInput.leftButton == Logic::True) {
            clickCount++;
            Panel& p = self.GetPanel();
            
            // Change color on click
            p.SetBufferBGColor(MakeColor(180, 70, 130));
            p.Title().text = "Clicked " + std::to_string(clickCount) + " times";
        }
    });
    
    app.GetDocument().AppendChildren(button);
    
    return app.Execute();
}
```

---

### Example 4: Hover Effect

Create an element that changes color on hover.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    
    Element hoverBox;
    hoverBox.Position() = MakeVector2D(15, 8);
    hoverBox.Dimension() = MakeVector2D(30, 8);
    hoverBox.Active() = Logic::True;
    
    Panel& panel = hoverBox.GetPanel();
    panel.Title().text = "Hover Over Me";
    panel.Title().visible = Logic::True;
    panel.SetBufferBGColor(MakeColor(60, 60, 60));
    panel.SetBufferFGColor(MakeColor(200, 200, 200));
    
    bool isHovered = false;
    
    hoverBox.DefineEventListener([&isHovered](EventImage& e, Element& self, Element& doc) {
        Panel& p = self.GetPanel();
        Vector2D mousePos = e.mouseInput.position;
        Vector2D elemPos = self.Position();
        Vector2D elemDim = self.Dimension();
        
        // Simple hover detection (check if mouse is within bounds)
        bool nowHovered = (mousePos.x >= elemPos.x && mousePos.x < elemPos.x + elemDim.x &&
                          mousePos.y >= elemPos.y && mousePos.y < elemPos.y + elemDim.y);
        
        if (nowHovered && !isHovered) {
            // Mouse entered
            p.SetBufferBGColor(MakeColor(120, 120, 120));
            p.Title().text = "Hovering!";
            isHovered = true;
        } else if (!nowHovered && isHovered) {
            // Mouse left
            p.SetBufferBGColor(MakeColor(60, 60, 60));
            p.Title().text = "Hover Over Me";
            isHovered = false;
        }
    });
    
    app.GetDocument().AppendChildren(hoverBox);
    
    return app.Execute();
}
```

---

### Example 5: Keyboard Input

Handle keyboard input events.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    
    Element inputBox;
    inputBox.Position() = MakeVector2D(10, 5);
    inputBox.Dimension() = MakeVector2D(50, 10);
    inputBox.Active() = Logic::True;
    
    Panel& panel = inputBox.GetPanel();
    panel.Title().text = "Type Something";
    panel.Title().visible = Logic::True;
    panel.SetBufferBGColor(MakeColor(40, 40, 40));
    panel.SetBufferFGColor(MakeColor(0, 255, 0));
    
    std::string typedText = "";
    
    inputBox.DefineEventListener([&typedText](EventImage& e, Element& self, Element& doc) {
        if (e.keyboardInput.keyPressed == Logic::True) {
            char key = e.keyboardInput.keyChar;
            
            if (key == '\b' && !typedText.empty()) {
                // Backspace
                typedText.pop_back();
            } else if (key >= 32 && key <= 126) {
                // Printable character
                typedText += key;
            }
            
            Panel& p = self.GetPanel();
            p.Title().text = "You typed: " + typedText;
        }
    });
    
    app.GetDocument().AppendChildren(inputBox);
    
    return app.Execute();
}
```

---

## Layout Examples

### Example 6: Nested Elements

Create a hierarchy of elements with parent-child relationships.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    
    // Parent container
    Element container;
    container.Position() = MakeVector2D(5, 5);
    container.Dimension() = MakeVector2D(70, 25);
    container.Active() = Logic::True;
    
    Panel& containerPanel = container.GetPanel();
    containerPanel.Title().text = "Container";
    containerPanel.Title().visible = Logic::True;
    containerPanel.SetBufferBGColor(MakeColor(30, 30, 60));
    containerPanel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Child elements
    for (int i = 0; i < 3; i++) {
        Element child;
        child.Position() = MakeVector2D(5, 5 + (i * 6));
        child.Dimension() = MakeVector2D(60, 5);
        child.Active() = Logic::True;
        
        Panel& childPanel = child.GetPanel();
        childPanel.Title().text = "Child " + std::to_string(i + 1);
        childPanel.Title().visible = Logic::True;
        childPanel.SetBufferBGColor(MakeColor(60, 60, 100 + (i * 30)));
        childPanel.SetBufferFGColor(MakeColor(255, 255, 255));
        
        container.AppendChildren(child);
    }
    
    app.GetDocument().AppendChildren(container);
    
    return app.Execute();
}
```

---

### Example 7: Custom Layout Handler

Implement custom layout logic with a layout handler.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    
    Element container;
    container.Position() = MakeVector2D(5, 3);
    container.Dimension() = MakeVector2D(80, 30);
    container.Active() = Logic::True;
    
    Panel& panel = container.GetPanel();
    panel.Title().text = "Auto-Layout Container";
    panel.Title().visible = Logic::True;
    
    // Add children
    for (int i = 0; i < 6; i++) {
        Element item;
        item.Dimension() = MakeVector2D(20, 5);
        item.Active() = Logic::True;
        
        Panel& itemPanel = item.GetPanel();
        itemPanel.Title().text = "Item " + std::to_string(i + 1);
        itemPanel.Title().visible = Logic::True;
        itemPanel.SetBufferBGColor(MakeColor(100 + (i * 20), 100, 150));
        
        container.AppendChildren(item);
    }
    
    // Define grid layout handler
    container.DefineLayoutHandler([](Element& parent, Element& self, Element& doc) {
        // Layout children in a grid (3 columns)
        int columns = 3;
        int spacing = 5;
        int itemWidth = 20;
        int itemHeight = 5;
        
        // This would require access to children (simplified example)
        // In practice, you'd iterate through children and position them
    });
    
    app.GetDocument().AppendChildren(container);
    
    return app.Execute();
}
```

---

## Styling Examples

### Example 8: Color Palette

Demonstrate various color combinations.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    Element& doc = app.GetDocument();
    
    struct ColorScheme {
        std::string name;
        Color bg;
        Color fg;
    };
    
    ColorScheme schemes[] = {
        {"Ocean", MakeColor(0, 105, 148), MakeColor(255, 255, 255)},
        {"Forest", MakeColor(34, 139, 34), MakeColor(255, 255, 224)},
        {"Sunset", MakeColor(255, 99, 71), MakeColor(255, 255, 255)},
        {"Night", MakeColor(25, 25, 112), MakeColor(173, 216, 230)},
        {"Desert", MakeColor(210, 180, 140), MakeColor(139, 69, 19)}
    };
    
    for (int i = 0; i < 5; i++) {
        Element box;
        box.Position() = MakeVector2D(5 + (i * 16), 5);
        box.Dimension() = MakeVector2D(15, 8);
        box.Active() = Logic::True;
        
        Panel& panel = box.GetPanel();
        panel.Title().text = schemes[i].name;
        panel.Title().visible = Logic::True;
        panel.SetBufferBGColor(schemes[i].bg);
        panel.SetBufferFGColor(schemes[i].fg);
        
        doc.AppendChildren(box);
    }
    
    return app.Execute();
}
```

---

### Example 9: Text with Custom Characters

Draw custom patterns using characters.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    
    Element canvas;
    canvas.Position() = MakeVector2D(10, 5);
    canvas.Dimension() = MakeVector2D(40, 15);
    canvas.Active() = Logic::True;
    
    Panel& panel = canvas.GetPanel();
    panel.Title().text = "ASCII Art Canvas";
    panel.Title().visible = Logic::True;
    panel.SetBufferBGColor(MakeColor(0, 0, 0));
    panel.SetBufferFGColor(MakeColor(0, 255, 0));
    
    // Draw a simple pattern
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 30; x++) {
            if ((x + y) % 2 == 0) {
                panel.SetPanelChar('#', x, y);
            } else {
                panel.SetPanelChar('.', x, y);
            }
        }
    }
    
    app.GetDocument().AppendChildren(canvas);
    
    return app.Execute();
}
```

---

## Advanced Examples

### Example 10: Menu System

Create a navigable menu system.

```cpp
#include "zim/zim.hpp"
#include <vector>
#include <string>

int main() {
    using namespace zim;
    
    Application app;
    Element& doc = app.GetDocument();
    
    // Menu data
    std::vector<std::string> menuItems = {
        "New Game",
        "Load Game",
        "Settings",
        "Credits",
        "Exit"
    };
    
    // Menu container
    Element menu;
    menu.Position() = MakeVector2D(20, 5);
    menu.Dimension() = MakeVector2D(40, 20);
    menu.Active() = Logic::True;
    
    Panel& menuPanel = menu.GetPanel();
    menuPanel.Title().text = "Main Menu";
    menuPanel.Title().visible = Logic::True;
    menuPanel.SetBufferBGColor(MakeColor(25, 25, 50));
    menuPanel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Selected item index
    int selectedIndex = 0;
    
    // Create menu items
    for (size_t i = 0; i < menuItems.size(); i++) {
        Element item;
        item.Id() = "menuItem_" + std::to_string(i);
        item.Position() = MakeVector2D(5, 3 + (i * 3));
        item.Dimension() = MakeVector2D(30, 2);
        item.Active() = Logic::True;
        
        Panel& itemPanel = item.GetPanel();
        itemPanel.Title().text = menuItems[i];
        itemPanel.Title().visible = Logic::True;
        itemPanel.SetBufferBGColor(
            i == selectedIndex ? MakeColor(70, 130, 180) : MakeColor(40, 40, 70)
        );
        itemPanel.SetBufferFGColor(MakeColor(255, 255, 255));
        
        // Add click handler
        item.DefineEventListener([i, &selectedIndex](EventImage& e, Element& self, Element& doc) {
            if (e.mouseInput.leftButton == Logic::True) {
                selectedIndex = i;
                Panel& p = self.GetPanel();
                p.SetBufferBGColor(MakeColor(70, 130, 180));
                p.Title().text = "> " + p.Title().text;
            }
        });
        
        menu.AppendChildren(item);
    }
    
    doc.AppendChildren(menu);
    
    return app.Execute();
}
```

---

### Example 11: Progress Bar

Animated progress indicator.

```cpp
#include "zim/zim.hpp"
#include <chrono>
#include <thread>

int main() {
    using namespace zim;
    
    Application app;
    
    Element progressContainer;
    progressContainer.Position() = MakeVector2D(15, 10);
    progressContainer.Dimension() = MakeVector2D(50, 7);
    progressContainer.Active() = Logic::True;
    
    Panel& panel = progressContainer.GetPanel();
    panel.Title().text = "Loading...";
    panel.Title().visible = Logic::True;
    panel.SetBufferBGColor(MakeColor(40, 40, 40));
    panel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Progress bar
    Element progressBar;
    progressBar.Position() = MakeVector2D(5, 3);
    progressBar.Dimension() = MakeVector2D(0, 2);  // Will grow
    progressBar.Active() = Logic::True;
    
    Panel& barPanel = progressBar.GetPanel();
    barPanel.SetBufferBGColor(MakeColor(0, 200, 0));
    
    progressContainer.AppendChildren(progressBar);
    app.GetDocument().AppendChildren(progressContainer);
    
    // Simulate progress (in real app, use proper event loop)
    int progress = 0;
    progressBar.DefineEventListener([&progress](EventImage& e, Element& self, Element& doc) {
        if (progress < 40) {
            progress++;
            self.Dimension().x = progress;
        }
    });
    
    return app.Execute();
}
```

---

### Example 12: Dashboard Layout

Create a dashboard with multiple panels.

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    Application app;
    Window& window = app.GetWindow();
    Element& doc = app.GetDocument();
    
    window.SetTitle("ZIM Dashboard");
    window.SetCursorVisibleLogic(Logic::False);
    
    // Header
    Element header;
    header.Position() = MakeVector2D(0, 0);
    header.Dimension() = MakeVector2D(80, 3);
    header.Active() = Logic::True;
    
    Panel& headerPanel = header.GetPanel();
    headerPanel.Title().text = "System Dashboard";
    headerPanel.Title().visible = Logic::True;
    headerPanel.SetBufferBGColor(MakeColor(30, 30, 100));
    headerPanel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Left panel - Stats
    Element statsPanel;
    statsPanel.Position() = MakeVector2D(2, 4);
    statsPanel.Dimension() = MakeVector2D(36, 20);
    statsPanel.Active() = Logic::True;
    
    Panel& stats = statsPanel.GetPanel();
    stats.Title().text = "Statistics";
    stats.Title().visible = Logic::True;
    stats.SetBufferBGColor(MakeColor(50, 50, 50));
    stats.SetBufferFGColor(MakeColor(200, 200, 200));
    
    // Right panel - Activity
    Element activityPanel;
    activityPanel.Position() = MakeVector2D(40, 4);
    activityPanel.Dimension() = MakeVector2D(36, 20);
    activityPanel.Active() = Logic::True;
    
    Panel& activity = activityPanel.GetPanel();
    activity.Title().text = "Recent Activity";
    activity.Title().visible = Logic::True;
    activity.SetBufferBGColor(MakeColor(50, 50, 50));
    activity.SetBufferFGColor(MakeColor(200, 200, 200));
    
    // Bottom panel - Status
    Element statusPanel;
    statusPanel.Position() = MakeVector2D(2, 25);
    statusPanel.Dimension() = MakeVector2D(74, 5);
    statusPanel.Active() = Logic::True;
    
    Panel& status = statusPanel.GetPanel();
    status.Title().text = "Status: Ready";
    status.Title().visible = Logic::True;
    status.SetBufferBGColor(MakeColor(0, 100, 0));
    status.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Build layout
    doc.AppendChildren(header);
    doc.AppendChildren(statsPanel);
    doc.AppendChildren(activityPanel);
    doc.AppendChildren(statusPanel);
    
    return app.Execute();
}
```

---

## Complete Application Examples

### Example 13: Simple Text Editor

A basic text editor interface.

```cpp
#include "zim/zim.hpp"
#include <string>
#include <vector>

int main() {
    using namespace zim;
    
    Application app;
    Window& window = app.GetWindow();
    Element& doc = app.GetDocument();
    
    window.SetTitle("ZIM Text Editor");
    window.SetCursorVisibleLogic(Logic::True);
    
    // Editor area
    Element editor;
    editor.Position() = MakeVector2D(2, 2);
    editor.Dimension() = MakeVector2D(76, 25);
    editor.Active() = Logic::True;
    
    Panel& editorPanel = editor.GetPanel();
    editorPanel.Title().text = "Untitled.txt";
    editorPanel.Title().visible = Logic::True;
    editorPanel.SetBufferBGColor(MakeColor(30, 30, 30));
    editorPanel.SetBufferFGColor(MakeColor(220, 220, 220));
    
    // Text buffer
    std::vector<std::string> lines;
    lines.push_back("");
    int currentLine = 0;
    int cursorX = 0;
    
    // Handle input
    editor.DefineEventListener([&](EventImage& e, Element& self, Element& doc) {
        if (e.keyboardInput.keyPressed == Logic::True) {
            char key = e.keyboardInput.keyChar;
            
            if (key == '\n') {
                // New line
                lines.push_back("");
                currentLine++;
                cursorX = 0;
            } else if (key == '\b') {
                // Backspace
                if (cursorX > 0) {
                    lines[currentLine].erase(cursorX - 1, 1);
                    cursorX--;
                }
            } else if (key >= 32 && key <= 126) {
                // Regular character
                lines[currentLine].insert(cursorX, 1, key);
                cursorX++;
            }
            
            // Update display (simplified)
            Panel& p = self.GetPanel();
            p.Title().text = "Line " + std::to_string(currentLine + 1) + 
                           ", Col " + std::to_string(cursorX + 1);
        }
    });
    
    // Status bar
    Element statusBar;
    statusBar.Position() = MakeVector2D(2, 28);
    statusBar.Dimension() = MakeVector2D(76, 2);
    statusBar.Active() = Logic::True;
    
    Panel& statusPanel = statusBar.GetPanel();
    statusPanel.Title().text = "Press keys to edit | Ctrl+S to save (not implemented)";
    statusPanel.Title().visible = Logic::True;
    statusPanel.SetBufferBGColor(MakeColor(0, 0, 100));
    statusPanel.SetBufferFGColor(MakeColor(200, 200, 200));
    
    doc.AppendChildren(editor);
    doc.AppendChildren(statusBar);
    
    return app.Execute();
}
```

---

## Tips for Building Complex UIs

1. **Modularize**: Create helper functions to generate common components
2. **State Management**: Use shared_ptr or global state for data that multiple elements need
3. **Event Delegation**: Use element IDs to find and update specific elements
4. **Performance**: Only update elements that change
5. **Layout**: Plan your layout on paper before coding

---

## More Resources

- [Getting Started Guide](GETTING_STARTED.md)
- [API Reference](API_REFERENCE.md)
- [Contributing](../CONTRIBUTING.md)

---

**Note**: These examples are for demonstration. In production code, add proper error handling, memory management, and input validation.
