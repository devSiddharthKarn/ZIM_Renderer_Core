# ZIM Core API Reference

Complete API documentation for ZIM Core library.

## Table of Contents

- [Namespace: zim](#namespace-zim)
- [Core Types](#core-types)
- [Application Class](#application-class)
- [Element Class](#element-class)
- [Window Class](#window-class)
- [Panel Class](#panel-class)
- [Input Types](#input-types)
- [Utility Functions](#utility-functions)

---

## Namespace: zim

All ZIM Core classes and functions are defined in the `zim` namespace.

```cpp
using namespace zim;
```

---

## Core Types

### Logic

Boolean enumeration for true/false values.

```cpp
enum class Logic : unsigned char {
    False = 0,
    True = 1
};
```

**Usage:**
```cpp
element.Active() = Logic::True;
```

---

### Vector2D

Represents a 2D coordinate or dimension.

```cpp
typedef struct {
    int x = 0;
    int y = 0;
} Vector2D;
```

**Functions:**
```cpp
Vector2D MakeVector2D(int x, int y);
Logic IsEqualVector2D(Vector2D vec1, Vector2D vec2);
```

**Example:**
```cpp
Vector2D position = MakeVector2D(10, 20);
Vector2D size = MakeVector2D(50, 30);
```

---

### Color

RGB color representation.

```cpp
struct Color {
    int r = 0;  // Red (0-255)
    int g = 0;  // Green (0-255)
    int b = 0;  // Blue (0-255)
};
```

**Functions:**
```cpp
Color MakeColor(int r, int g, int b);
Logic IsEqualColor(Color color1, Color color2);
```

**Example:**
```cpp
Color red = MakeColor(255, 0, 0);
Color blue = MakeColor(0, 0, 255);
Color white = MakeColor(255, 255, 255);
```

---

### OverloadStyle

Text styling options that can be combined using bitwise OR.

```cpp
enum class OverloadStyle : unsigned long long {
    None = 0,
    
    // Style options
    Bold = 1ULL << 0,
    Dim = 1ULL << 1,
    Italic = 1ULL << 2,
    Underline = 1ULL << 3,
    BlinkSlow = 1ULL << 4,
    BlinkFast = 1ULL << 5,
    Reverse = 1ULL << 6,
    Hidden = 1ULL << 7,
    Strikethrough = 1ULL << 8,
    Framed = 1ULL << 9,
    Encircled = 1ULL << 10,
    Overline = 1ULL << 11,
    Superscript = 1ULL << 12,
    Subscript = 1ULL << 13,
    
    // Style resets
    NormalIntensity = 1ULL << 14,
    NotItalic = 1ULL << 15,
    UnderlineOff = 1ULL << 16,
    BlinkOff = 1ULL << 17,
    ReverseOff = 1ULL << 18,
    Visible = 1ULL << 19,
    StrikethroughOff = 1ULL << 20,
    FramedOff = 1ULL << 21,
    EncircledOff = 1ULL << 22,
    OverlineOff = 1ULL << 23,
    
    ResetBit = 1ULL << 24
};
```

**Operators:**
```cpp
OverloadStyle operator|(OverloadStyle a, OverloadStyle b);  // Combine styles
OverloadStyle operator&(OverloadStyle a, OverloadStyle b);  // Check styles
OverloadStyle operator~(OverloadStyle a);                    // Invert
OverloadStyle operator^(OverloadStyle a, OverloadStyle b);  // XOR
```

**Example:**
```cpp
OverloadStyle style = OverloadStyle::Bold | OverloadStyle::Underline;
```

---

### PointDetail

Detailed information for a single character position.

```cpp
struct PointDetail {
    char ch;              // Character
    Color fgColor;        // Foreground color
    Color bgColor;        // Background color
    OverloadStyle style;  // Text style
};
```

---

### StringDetail

String with visibility control.

```cpp
struct StringDetail {
    std::string text;
    Logic visible;
};
```

---

### EventImage

Contains information about input events.

```cpp
struct EventImage {
    MouseInput mouseInput;
    KeyboardInput keyboardInput;
};
```

---

## Application Class

The main application class that manages the window and document.

### Constructor

```cpp
Application();
```

Creates a new ZIM application instance.

### Methods

#### GetWindow()

```cpp
Window& GetWindow();
```

Returns a reference to the application's window.

**Example:**
```cpp
Window& window = app.GetWindow();
Vector2D dims = window.GetDimensions();
```

---

#### GetDocument()

```cpp
Element& GetDocument();
```

Returns the root document element. All visible elements should be added as children of the document.

**Example:**
```cpp
Element& doc = app.GetDocument();
doc.AppendChildren(myElement);
```

---

#### Execute()

```cpp
int Execute();
```

Starts the application event loop. This is blocking and will run until the application exits.

**Returns:** Exit code (0 for success)

**Example:**
```cpp
return app.Execute();
```

---

### Destructor

```cpp
~Application();
```

Cleans up application resources.

---

## Element Class

Represents a UI element that can contain children and handle events.

### Constructor

```cpp
Element();
```

Creates a new UI element.

### Methods

#### AppendChildren()

```cpp
void AppendChildren(Element& element);
```

Adds a child element to this element.

**Parameters:**
- `element`: The child element to add

**Example:**
```cpp
Element parent;
Element child;
parent.AppendChildren(child);
```

---

#### GetPanel()

```cpp
Panel& GetPanel();
```

Returns the panel associated with this element for rendering customization.

**Example:**
```cpp
Panel& panel = element.GetPanel();
panel.SetBufferBGColor(MakeColor(0, 0, 255));
```

---

#### DefineEventListener()

```cpp
void DefineEventListener(EventListener listener);
```

Attaches an event handler to this element.

**Parameters:**
- `listener`: Function to handle events

**Signature:**
```cpp
using EventListener = std::function<void(EventImage& eventImage, Element& This, Element& document)>;
```

**Example:**
```cpp
element.DefineEventListener([](EventImage& e, Element& self, Element& doc) {
    if (e.mouseInput.leftButton == Logic::True) {
        // Handle click
    }
});
```

---

#### ProcessEventListener()

```cpp
void ProcessEventListener(EventImage& eventImage, Element& document);
```

Processes events for this element. Usually called internally.

---

#### DefineLayoutHandler()

```cpp
void DefineLayoutHandler(ElementLayoutHandler layoutHandler);
```

Defines custom layout logic for positioning child elements.

**Signature:**
```cpp
using ElementLayoutHandler = std::function<void(Element& parent, Element& This, Element& document)>;
```

**Example:**
```cpp
element.DefineLayoutHandler([](Element& parent, Element& self, Element& doc) {
    // Custom layout code
    self.Position() = MakeVector2D(0, 0);
});
```

---

#### ComputeLayout()

```cpp
void ComputeLayout(Element& parent, Element& document);
```

Computes layout for this element and its children.

---

#### Active()

```cpp
Logic& Active();
```

Returns reference to element's active state. Element must be active to be visible and interactive.

**Example:**
```cpp
element.Active() = Logic::True;
```

---

#### Position()

```cpp
Vector2D& Position();
```

Returns reference to element's position relative to its parent.

**Example:**
```cpp
element.Position() = MakeVector2D(10, 10);
int x = element.Position().x;
```

---

#### Dimension()

```cpp
Vector2D& Dimension();
```

Returns reference to element's dimensions (width, height).

**Example:**
```cpp
element.Dimension() = MakeVector2D(50, 20);
int width = element.Dimension().x;
int height = element.Dimension().y;
```

---

#### Id()

```cpp
std::string& Id();
```

Returns reference to element's ID string.

**Example:**
```cpp
element.Id() = "myButton";
```

---

#### FindElementById()

```cpp
Element* FindElementById(std::string id);
```

Searches for a child element with the specified ID.

**Parameters:**
- `id`: Element ID to search for

**Returns:** Pointer to element if found, `nullptr` otherwise

**Example:**
```cpp
Element* found = document.FindElementById("myElement");
if (found != nullptr) {
    found->Position() = MakeVector2D(20, 20);
}
```

---

#### ClearBuffers()

```cpp
void ClearBuffers();
```

Clears this element's render buffers.

---

#### Render()

```cpp
void Render();
```

Renders this element to its panel.

---

#### ReadyForWindowContext()

```cpp
void ReadyForWindowContext(Window& windowContext);
```

Prepares element for rendering in the given window context.

---

### Destructor

```cpp
~Element();
```

Cleans up element resources.

---

## Window Class

Manages the terminal window and rendering.

### Constructor

```cpp
Window(PointDetail bufferBaseDetail);
```

Creates a window with the specified default buffer detail.

**Parameters:**
- `bufferBaseDetail`: Default character detail for the buffer

---

### Methods

#### CaptureEventImage()

```cpp
void CaptureEventImage();
```

Captures current input events (keyboard, mouse).

---

#### GetEventImage()

```cpp
EventImage& GetEventImage() const;
```

Returns the current event image containing input data.

**Example:**
```cpp
EventImage& events = window.GetEventImage();
if (events.mouseInput.leftButton == Logic::True) {
    // Handle click
}
```

---

#### GetBufferBaseDetail()

```cpp
const PointDetail& GetBufferBaseDetail();
```

Returns the default buffer point detail.

---

#### SetPointDetail()

```cpp
void SetPointDetail(int position, PointDetail detail);
void SetPointDetail(Vector2D position, PointDetail detail);
```

Sets the character detail at a specific position in the window buffer.

**Parameters:**
- `position`: Linear or 2D position
- `detail`: Character detail to set

**Example:**
```cpp
PointDetail detail;
detail.ch = 'A';
detail.fgColor = MakeColor(255, 255, 255);
detail.bgColor = MakeColor(0, 0, 0);
window.SetPointDetail(MakeVector2D(10, 5), detail);
```

---

#### GetPointDetail()

```cpp
PointDetail GetPointDetail(int position);
PointDetail GetPointDetail(Vector2D position);
```

Gets the character detail at a specific position.

---

#### SetTitle()

```cpp
void SetTitle(std::string title);
```

Sets the window title.

**Example:**
```cpp
window.SetTitle("My ZIM Application");
```

---

#### GetDimensions()

```cpp
Vector2D GetDimensions();
```

Returns the current window dimensions.

**Example:**
```cpp
Vector2D dims = window.GetDimensions();
int width = dims.x;
int height = dims.y;
```

---

#### Refresh()

```cpp
void Refresh();
```

Updates window dimensions (call after terminal resize).

---

#### SetCursorVisibleLogic()

```cpp
void SetCursorVisibleLogic(Logic logic);
```

Shows or hides the cursor.

**Example:**
```cpp
window.SetCursorVisibleLogic(Logic::False);  // Hide cursor
```

---

#### HardClearBuffer()

```cpp
void HardClearBuffer();
```

Completely clears the window buffer.

---

#### SoftClearBuffer()

```cpp
void SoftClearBuffer();
```

Clears buffer while preserving some state.

---

#### Render()

```cpp
void Render(Logic enableDiffing = Logic::True);
```

Renders the buffer to the terminal.

**Parameters:**
- `enableDiffing`: Enable differential rendering (only update changed characters)

---

#### StoreCommandBuffer()

```cpp
void StoreCommandBuffer(std::string filename, Logic overWrite = Logic::False);
```

Saves rendering commands to a file for debugging.

**Parameters:**
- `filename`: Output file path
- `overWrite`: Whether to overwrite existing file

---

#### Retrace()

```cpp
void Retrace();
```

Recomputes window rendering state.

---

### Destructor

```cpp
~Window();
```

Cleans up window resources.

---

## Panel Class

A rectangular UI component for rendering content.

### Constructor

```cpp
Panel(Vector2D position, Vector2D dimension);
```

Creates a panel at the specified position with given dimensions.

---

### Methods

#### Title()

```cpp
StringDetail& Title();
```

Returns reference to panel's title.

**Example:**
```cpp
panel.Title().text = "My Panel";
panel.Title().visible = Logic::True;
```

---

#### SetBufferBGColor()

```cpp
void SetBufferBGColor(Color color);
```

Sets the panel's background color.

---

#### SetBufferFGColor()

```cpp
void SetBufferFGColor(Color color);
```

Sets the panel's foreground (text) color.

---

#### Resize()

```cpp
void Resize(Vector2D dimension);
```

Resizes the panel.

---

#### SetPanelChar()

```cpp
void SetPanelChar(char ch, int x, int y);
```

Sets a character at a specific position in the panel.

---

#### GetPointDetail()

```cpp
const PointDetail GetPointDetail(int position);
```

Gets character detail at position.

---

#### SetPointDetail()

```cpp
void SetPointDetail(Vector2D position, PointDetail detail);
```

Sets character detail at position.

---

#### BindWindowContext()

```cpp
void BindWindowContext(Window& window);
```

Binds panel to a window for rendering.

---

#### Retrace()

```cpp
void Retrace();
```

Recomputes panel rendering.

---

#### RetraceBuffer()

```cpp
void RetraceBuffer();
```

Recomputes panel buffer.

---

#### RetraceBorders()

```cpp
void RetraceBorders();
```

Recomputes panel borders.

---

#### RefreshBufferDimensions()

```cpp
void RefreshBufferDimensions();
```

Updates buffer dimensions after resize.

---

## Input Types

### MouseInput

```cpp
struct MouseInput {
    Vector2D position;       // Mouse position
    Logic leftButton;        // Left button state
    Logic rightButton;       // Right button state
    Logic middleButton;      // Middle button state
    // Additional fields...
};
```

### KeyboardInput

```cpp
struct KeyboardInput {
    Logic keyPressed;        // Whether a key is pressed
    char keyChar;           // Character pressed
    int keyCode;            // Key code
    // Additional fields...
};
```

---

## Utility Functions

### Vector Math

```cpp
Vector2D MakeVector2D(int x, int y);
Logic IsEqualVector2D(Vector2D vec1, Vector2D vec2);
```

### Color Functions

```cpp
Color MakeColor(int r, int g, int b);
Logic IsEqualColor(Color color1, Color color2);
```

---

## Complete Example

Here's a comprehensive example using various APIs:

```cpp
#include "zim/zim.hpp"

int main() {
    using namespace zim;
    
    // Create application
    Application app;
    Window& window = app.GetWindow();
    Element& document = app.GetDocument();
    
    // Configure window
    window.SetTitle("ZIM API Demo");
    window.SetCursorVisibleLogic(Logic::False);
    
    // Create main container
    Element container;
    container.Id() = "container";
    container.Position() = MakeVector2D(5, 5);
    container.Dimension() = MakeVector2D(60, 20);
    container.Active() = Logic::True;
    
    // Style the container
    Panel& containerPanel = container.GetPanel();
    containerPanel.Title().text = "Main Container";
    containerPanel.Title().visible = Logic::True;
    containerPanel.SetBufferBGColor(MakeColor(30, 30, 60));
    containerPanel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Create button
    Element button;
    button.Id() = "button1";
    button.Position() = MakeVector2D(10, 10);
    button.Dimension() = MakeVector2D(20, 3);
    button.Active() = Logic::True;
    
    Panel& buttonPanel = button.GetPanel();
    buttonPanel.Title().text = "Click Me";
    buttonPanel.Title().visible = Logic::True;
    buttonPanel.SetBufferBGColor(MakeColor(100, 100, 100));
    buttonPanel.SetBufferFGColor(MakeColor(255, 255, 255));
    
    // Add event listener
    button.DefineEventListener([](EventImage& e, Element& self, Element& doc) {
        if (e.mouseInput.leftButton == Logic::True) {
            Panel& p = self.GetPanel();
            p.SetBufferBGColor(MakeColor(200, 100, 100));
            p.Title().text = "Clicked!";
        }
    });
    
    // Build hierarchy
    container.AppendChildren(button);
    document.AppendChildren(container);
    
    // Run application
    return app.Execute();
}
```

---

## Platform-Specific Notes

### Windows

- Window rendering uses Windows Console API
- Full color support (RGB)
- Mouse and keyboard input supported

### Future Platforms

- Linux: ANSI escape sequences
- macOS: Terminal API

---

## Best Practices

1. **Always set Active() = Logic::True** for visible elements
2. **Use const references** when passing large objects
3. **Check null pointers** when using FindElementById()
4. **Set dimensions before position** for predictable layout
5. **Bind panels to windows** before rendering
6. **Use event listeners** for interactive elements

---

## Version Information

- **API Version**: 1.0
- **C++ Standard**: C++11 or higher
- **Platform**: Windows (Linux/macOS in development)

---

For more information, see:
- [Getting Started Guide](GETTING_STARTED.md)
- [Contributing Guidelines](../CONTRIBUTING.md)
- [GitHub Repository](https://github.com/YOUR_USERNAME/ZIM_core)
