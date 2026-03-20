#include "../zim/zim.hpp"

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
    ui.Position() = MakeVector2D(2, 1);
    ui.Dimension() = MakeVector2D(80, 12);
    ui.GetPanel().SetPadding(1);
    ui.GetPanel().Title().sentence = "Input test (press q or Esc to quit)";

    std::string lastKey = "None";
    std::string lastModifier = "None";
    std::string lastMouse = "None";
    Vector2D lastMousePos = MakeVector2D(0, 0);
    Vector2D lastScroll = MakeVector2D(0, 0);

    ui.DefineEventListener([&](EventImage &eventImage, Element &This, Element &document) {
        if (eventImage.eventOccuredLogic == Logic::True)
        {
            while (eventImage.keyboardEvent.keys.IsEmpty() == Logic::False)
            {
                KeyState state = eventImage.keyboardEvent.keys.Get();
                if (state.key == KeyboardKey::None)
                {
                    continue;
                }

                lastKey = KeyToString(state.key);
                lastModifier = KeyToString(state.modifier);

                if (state.key == KeyboardKey::Escape || state.key == KeyboardKey::q || state.key == KeyboardKey::Q)
                {
                    app.SafeQuit();
                }
            }

            if (eventImage.mouseEvent.keyPressed != MouseKey::None)
            {
                lastMouse = MouseToString(eventImage.mouseEvent.keyPressed);
            }

            if (eventImage.mouseEvent.position.x != 0 || eventImage.mouseEvent.position.y != 0)
            {
                lastMousePos = eventImage.mouseEvent.position;
            }

            if (eventImage.mouseEvent.scroll.x != 0 || eventImage.mouseEvent.scroll.y != 0)
            {
                lastScroll = eventImage.mouseEvent.scroll;
            }
        }

        This.GetPanel().ClearBuffer();
        This.GetPanel().WriteStr("Keyboard + Mouse Input Test", 0, 0, Logic::True);
        This.GetPanel().WriteStr("Quit: q / Esc", 0, 1, Logic::True);
        This.GetPanel().WriteStr("Last key      : " + lastKey, 0, 3, Logic::True);
        This.GetPanel().WriteStr("Last modifier : " + lastModifier, 0, 4, Logic::True);
        This.GetPanel().WriteStr("Last mouse btn: " + lastMouse, 0, 6, Logic::True);
        This.GetPanel().WriteStr(
            "Mouse position: (" + std::to_string(lastMousePos.x) + ", " + std::to_string(lastMousePos.y) + ")", 0, 7, Logic::True);
        This.GetPanel().WriteStr(
            "Last scroll   : (" + std::to_string(lastScroll.x) + ", " + std::to_string(lastScroll.y) + ")", 0, 8, Logic::True);
    });

    app.GetDocument().AppendChildren(ui);

    return app.Execute();
}
