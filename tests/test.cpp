#include "../zim/zim.hpp"

#include <deque>
#include <string>

using namespace zim;

static std::string KeyToString(KeyboardKey key)
{
    const int value = static_cast<int>(key);
    if (value >= 32 && value <= 126)
    {
        return std::string(1, static_cast<char>(value));
    }

    switch (key)
    {
    case KeyboardKey::None:
        return "None";
    case KeyboardKey::Enter:
        return "Enter";
    case KeyboardKey::Tab:
        return "Tab";
    case KeyboardKey::Escape:
        return "Escape";
    case KeyboardKey::Backspace:
        return "Backspace";
    case KeyboardKey::ArrowUp:
        return "ArrowUp";
    case KeyboardKey::ArrowDown:
        return "ArrowDown";
    case KeyboardKey::ArrowLeft:
        return "ArrowLeft";
    case KeyboardKey::ArrowRight:
        return "ArrowRight";
    case KeyboardKey::Home:
        return "Home";
    case KeyboardKey::End:
        return "End";
    case KeyboardKey::PageUp:
        return "PageUp";
    case KeyboardKey::PageDown:
        return "PageDown";
    case KeyboardKey::Insert:
        return "Insert";
    case KeyboardKey::Delete:
        return "Delete";
    case KeyboardKey::F1:
        return "F1";
    case KeyboardKey::F2:
        return "F2";
    case KeyboardKey::F3:
        return "F3";
    case KeyboardKey::F4:
        return "F4";
    case KeyboardKey::F5:
        return "F5";
    case KeyboardKey::F6:
        return "F6";
    case KeyboardKey::F7:
        return "F7";
    case KeyboardKey::F8:
        return "F8";
    case KeyboardKey::F9:
        return "F9";
    case KeyboardKey::F10:
        return "F10";
    case KeyboardKey::F11:
        return "F11";
    case KeyboardKey::F12:
        return "F12";
    case KeyboardKey::LeftCtrl:
        return "LeftCtrl";
    case KeyboardKey::RightCtrl:
        return "RightCtrl";
    case KeyboardKey::LeftAlt:
        return "LeftAlt";
    case KeyboardKey::RightAlt:
        return "RightAlt";
    case KeyboardKey::Shift:
        return "Shift";
    default:
        return "Unknown";
    }
}

static std::string MouseToString(MouseKey key)
{
    switch (key)
    {
    case MouseKey::Left:
        return "Left";
    case MouseKey::Right:
        return "Right";
    case MouseKey::Middle:
        return "Middle";
    default:
        return "None";
    }
}

int main()
{
    Application app;

    Element ui;
    ui.Position() = MakeVector2D(1, 1);
    ui.Dimension() = MakeVector2D(100, 18);
    ui.GetPanel().SetPadding(1);
    ui.GetPanel().Title().sentence = "Live Input Feed (q or Esc to quit)";

    std::string lastKey = "None";
    std::string lastModifier = "None";
    std::string lastMouseButton = "None";
    Vector2D mousePos = MakeVector2D(0, 0);
    Vector2D prevMousePos = MakeVector2D(0, 0);
    Vector2D mouseDelta = MakeVector2D(0, 0);
    Vector2D lastScroll = MakeVector2D(0, 0);
    int eventCount = 0;
    std::deque<std::string> liveLog;

    ui.DefineEventListener([&](EventImage &eventImage, Element &This, Element &document) {
        if (eventImage.eventOccuredLogic == Logic::True)
        {
            eventCount++;

            if (eventImage.mouseEvent.position.x != 0 || eventImage.mouseEvent.position.y != 0)
            {
                prevMousePos = mousePos;
                mousePos = eventImage.mouseEvent.position;
                mouseDelta = MakeVector2D(mousePos.x - prevMousePos.x, mousePos.y - prevMousePos.y);
                liveLog.push_back("Mouse move -> (" + std::to_string(mousePos.x) + "," + std::to_string(mousePos.y) + ")");
            }

            if (eventImage.mouseEvent.keyPressed != MouseKey::None)
            {
                lastMouseButton = MouseToString(eventImage.mouseEvent.keyPressed);
                liveLog.push_back("Mouse btn  -> " + lastMouseButton);
            }

            if (eventImage.mouseEvent.scroll.x != 0 || eventImage.mouseEvent.scroll.y != 0)
            {
                lastScroll = eventImage.mouseEvent.scroll;
                liveLog.push_back("Scroll     -> (" + std::to_string(lastScroll.x) + "," + std::to_string(lastScroll.y) + ")");
            }

            while (eventImage.keyboardEvent.keys.IsEmpty() == Logic::False)
            {
                const KeyState state = eventImage.keyboardEvent.keys.Get();
                if (state.key == KeyboardKey::None)
                {
                    continue;
                }

                lastKey = KeyToString(state.key);
                lastModifier = KeyToString(state.modifier);
                liveLog.push_back("Key        -> " + lastKey + " | mod: " + lastModifier);

                if (state.key == KeyboardKey::Escape || state.key == KeyboardKey::q || state.key == KeyboardKey::Q)
                {
                    app.SafeQuit();
                }
            }
        }

        while (liveLog.size() > 6)
        {
            liveLog.pop_front();
        }

        This.GetPanel().ClearBuffer();
        This.GetPanel().WriteStr("Live keyboard/mouse feedback", 0, 0, Logic::True);
        This.GetPanel().WriteStr("Try: arrows, shift+arrows, ctrl+a, clicks, wheel", 0, 1, Logic::True);
        This.GetPanel().WriteStr("Quit: q / Esc", 0, 2, Logic::True);
        This.GetPanel().WriteStr("Event count : " + std::to_string(eventCount), 0, 4, Logic::True);
        This.GetPanel().WriteStr("Last key    : " + lastKey, 0, 5, Logic::True);
        This.GetPanel().WriteStr("Last mod    : " + lastModifier, 0, 6, Logic::True);
        This.GetPanel().WriteStr("Mouse button: " + lastMouseButton, 0, 7, Logic::True);
        This.GetPanel().WriteStr("Mouse pos   : (" + std::to_string(mousePos.x) + ", " + std::to_string(mousePos.y) + ")", 0, 8, Logic::True);
        This.GetPanel().WriteStr("Mouse delta : (" + std::to_string(mouseDelta.x) + ", " + std::to_string(mouseDelta.y) + ")", 0, 9, Logic::True);
        This.GetPanel().WriteStr("Last scroll : (" + std::to_string(lastScroll.x) + ", " + std::to_string(lastScroll.y) + ")", 0, 10, Logic::True);
        This.GetPanel().WriteStr("Live log:", 0, 12, Logic::True);

        int row = 13;
        for (const std::string &line : liveLog)
        {
            This.GetPanel().WriteStr(line, 0, row, Logic::True);
            row++;
        }
    });

    app.GetDocument().AppendChildren(ui);
    return app.Execute();
}
