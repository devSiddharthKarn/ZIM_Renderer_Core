#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "../Window.hpp"

#include "windows.h"
#include <conio.h>

#include "iostream"
#include "fstream"
#include "unordered_map"
#include "queue"

void set_special_console_flags_win32(HANDLE inputHandle)
{

    // enable mouse events
    DWORD mode;
    GetConsoleMode(inputHandle, &mode);

    mode |= ENABLE_EXTENDED_FLAGS;
    mode |= ENABLE_MOUSE_INPUT;
    mode |= ENABLE_WINDOW_INPUT;
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    mode &= ~ENABLE_QUICK_EDIT_MODE;

    SetConsoleMode(inputHandle, mode);
}

namespace zim
{
    struct Window::Impl_Window
    {
        std::string title;

        HANDLE o_handle;
        HANDLE i_handle;

        Vector2D dimen;

        PointDetail bufferBaseDetail;

        std::vector<PointDetail> frameBuffer;

        std::vector<INPUT_RECORD> input_records;

        EventImage event_image;

        std::string command_buffer;

        std::unordered_map<int, PointDetail> dirtyPoints;
    };

    void InputHandleKeyEvents(KEY_EVENT_RECORD &keyEvent_win32, EventImage &eventImage)
    {

        // Only process key DOWN events (ignore key up)

        if (keyEvent_win32.bKeyDown)
        {
            eventImage.eventOccuredLogic = Logic::True;

            // Get the virtual key code
            WORD vk = keyEvent_win32.wVirtualKeyCode;

            // Convert to KeyboardKey
            KeyboardKey key = KeyboardKey::Unknown;

            // Arrow keys
            if (vk == VK_UP)
                key = KeyboardKey::ArrowUp;
            else if (vk == VK_DOWN)
                key = KeyboardKey::ArrowDown;
            else if (vk == VK_LEFT)
                key = KeyboardKey::ArrowLeft;
            else if (vk == VK_RIGHT)
                key = KeyboardKey::ArrowRight;

            // Special keys
            else if (vk == VK_RETURN)
                key = KeyboardKey::Enter;
            else if (vk == VK_BACK)
                key = KeyboardKey::Backspace;
            else if (vk == VK_TAB)
                key = KeyboardKey::Tab;
            else if (vk == VK_ESCAPE)
                key = KeyboardKey::Escape;
            else if (vk == VK_SPACE)
                key = KeyboardKey::Space;
            else if (vk == VK_DELETE)
                key = KeyboardKey::Delete;
            else if (vk == VK_INSERT)
                key = KeyboardKey::Insert;
            else if (vk == VK_HOME)
                key = KeyboardKey::Home;
            else if (vk == VK_END)
                key = KeyboardKey::End;
            else if (vk == VK_PRIOR)
                key = KeyboardKey::PageUp;
            else if (vk == VK_NEXT)
                key = KeyboardKey::PageDown;

            // Function keys
            else if (vk >= VK_F1 && vk <= VK_F12)
            {
                key = static_cast<KeyboardKey>(
                    static_cast<int>(KeyboardKey::F1) + (vk - VK_F1));
            }

            // ASCII characters (use the character, not virtual key)
            else if (keyEvent_win32.uChar.AsciiChar >= 33 && keyEvent_win32.uChar.AsciiChar <= 126)
            {
                key = static_cast<KeyboardKey>(keyEvent_win32.uChar.AsciiChar);
            }

            DWORD controlState = keyEvent_win32.dwControlKeyState;

            KeyboardKey modifier = KeyboardKey::None;

            if (controlState & LEFT_CTRL_PRESSED)
            {
                modifier = KeyboardKey::LeftCtrl;
                // Use virtual key code for letters when Ctrl is pressed
                if (vk >= 'A' && vk <= 'Z')
                {
                    key = static_cast<KeyboardKey>(vk + 32); // Convert VK to lowercase letter
                }
            }
            else if (controlState & RIGHT_CTRL_PRESSED)
            {
                modifier = KeyboardKey::RightCtrl;
                if (vk >= 'A' && vk <= 'Z')
                {
                    key = static_cast<KeyboardKey>(vk + 32); // Convert VK to lowercase letter
                }
            }
            else if (controlState & SHIFT_PRESSED)
            {
                modifier = KeyboardKey::Shift;
            }
            else if (controlState & LEFT_ALT_PRESSED)
            {
                modifier = KeyboardKey::LeftAlt;
            }
            else if (controlState & RIGHT_ALT_PRESSED)
            {
                modifier = KeyboardKey::RightAlt;
            }
            else if (controlState & CAPSLOCK_ON)
            {
                modifier = KeyboardKey::Capslock_on;
            }
            else if (controlState & NUMLOCK_ON)
            {
                modifier = KeyboardKey::Numlock_on;
            }
            else if (controlState & SCROLLLOCK_ON)
            {
                modifier = KeyboardKey::Scrolllock_on;
            }
            else
            {
                modifier = KeyboardKey::None;
            }

            if (key != KeyboardKey::Unknown)
            {
                eventImage.keyboardEvent.keys.Put(KeyState(key, modifier));
            }
        }
    }

    void InputHandleMouseEvents(MOUSE_EVENT_RECORD &m_record, EventImage &eventImage)
    {

        eventImage.eventOccuredLogic = Logic::True;
        eventImage.mouseEvent.position = MakeVector2D(
            m_record.dwMousePosition.X,
            m_record.dwMousePosition.Y);

        // Handle mouse buttons if needed
        if (m_record.dwEventFlags == 0)
        {
            if (m_record.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
            {
                eventImage.mouseEvent.keyPressed = MouseKey::Left;
            }
            if (m_record.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
            {
                eventImage.mouseEvent.keyPressed = MouseKey::Right;
            }

            if (m_record.dwButtonState & FROM_LEFT_2ND_BUTTON_PRESSED)
            {
                eventImage.mouseEvent.keyPressed = MouseKey::Middle;
            }
        }
        else if (m_record.dwEventFlags == MOUSE_WHEELED)
        {
            SHORT wheelDelta = HIWORD(m_record.dwButtonState);

            if (wheelDelta > 0)
            {
                eventImage.mouseEvent.scroll.y = 1;
            }
            else if (wheelDelta < 0)
            {
                eventImage.mouseEvent.scroll.y = -1;
            }
        }
        else if (m_record.dwEventFlags == MOUSE_HWHEELED)
        {
            SHORT wheelDelta = HIWORD(m_record.dwButtonState);

            if (wheelDelta > 0)
            {
                eventImage.mouseEvent.scroll.x = 1;
            }
            else if (wheelDelta < 0)
            {
                eventImage.mouseEvent.scroll.x = -1;
            }
        }
    }

    void InputHandleWindowBufferSizeEvents(WINDOW_BUFFER_SIZE_RECORD &record, EventImage &eventImage, Vector2D &old_dimension)
    {
        int new_x = record.dwSize.X;
        int new_y = record.dwSize.Y;

        if (new_x != old_dimension.x || new_y != old_dimension.y)
        {
            eventImage.windowEvent.isResizedLogic = Logic::True;
        }
        return;
    }

    void InputHandleWindowFocusEvents(FOCUS_EVENT_RECORD &focusEventRecord, EventImage &eventImage)
    {
        if (focusEventRecord.bSetFocus == TRUE)
        {
            eventImage.windowEvent.isFocusedLogic = Logic::True;
        }
        else
        {
            eventImage.windowEvent.isFocusedLogic = Logic::False;
        }
    }

    void Window::CaptureEventImage()
    {
        EventImage &eventImage = this->pImpl_Window->event_image;

        eventImage.eventOccuredLogic = Logic::False;

        eventImage.keyboardEvent.keys.MakeEmpty();

        eventImage.mouseEvent.keyPressed = MouseKey::None;
        eventImage.mouseEvent.scroll = MakeVector2D(0, 0);

        eventImage.windowEvent.isFocusedLogic = Logic::True;
        eventImage.windowEvent.isResizedLogic = Logic::False;

        DWORD eventsRead;
        DWORD eventsAvailable;

        GetNumberOfConsoleInputEvents(this->pImpl_Window->i_handle, &eventsAvailable);

        if (eventsAvailable > 0)
        {
            DWORD toRead = eventsAvailable;
            this->pImpl_Window->input_records.resize(eventsAvailable);

            if (ReadConsoleInput(this->pImpl_Window->i_handle, this->pImpl_Window->input_records.data(), toRead, &eventsRead))
            {
                for (DWORD i = 0; i < eventsRead; i++)
                {
                    if (this->pImpl_Window->input_records[i].EventType == KEY_EVENT)
                    {
                        KEY_EVENT_RECORD &keyEvent = this->pImpl_Window->input_records[i].Event.KeyEvent;

                        InputHandleKeyEvents(keyEvent, eventImage);
                    }
                    else if (this->pImpl_Window->input_records[i].EventType == MOUSE_EVENT)
                    {
                        MOUSE_EVENT_RECORD &m_record = this->pImpl_Window->input_records[i].Event.MouseEvent;

                        InputHandleMouseEvents(m_record, eventImage);
                    }
                    else if (this->pImpl_Window->input_records[i].EventType == WINDOW_BUFFER_SIZE_EVENT)
                    {
                        WINDOW_BUFFER_SIZE_RECORD &window_buf_size_record = this->pImpl_Window->input_records[i].Event.WindowBufferSizeEvent;

                        InputHandleWindowBufferSizeEvents(window_buf_size_record, eventImage, this->pImpl_Window->dimen);
                    }
                    else if (this->pImpl_Window->input_records[i].EventType == FOCUS_EVENT)
                    {
                        FOCUS_EVENT_RECORD &focusEventRecord = this->pImpl_Window->input_records[i].Event.FocusEvent;

                        InputHandleWindowFocusEvents(focusEventRecord, eventImage);
                    }
                }
            }
        }
    }

    EventImage &Window::GetEventImage() const
    {
        return this->pImpl_Window->event_image;
    }

    void Window::SetPointDetail(int position, PointDetail detail)
    {
        if (position < 0 || position >= this->pImpl_Window->dimen.x * this->pImpl_Window->dimen.y)
            return;

        this->pImpl_Window->dirtyPoints[position] = detail;
        this->pImpl_Window->frameBuffer[position] = detail;
    }

    void Window::SetPointDetail(Vector2D position, PointDetail detail)
    {
        int pos = position.y * this->pImpl_Window->dimen.x + position.x;

        if (pos < 0 || pos >= this->pImpl_Window->dimen.x * this->pImpl_Window->dimen.y)
            return;

        this->pImpl_Window->dirtyPoints[pos] = detail;
    }

    void Window::Retrace()
    {

        // Clear the entire framebuffer by assigning new values to all positions
        this->pImpl_Window->frameBuffer.assign(this->pImpl_Window->dimen.x * this->pImpl_Window->dimen.y, this->pImpl_Window->bufferBaseDetail);

        for (int i = 0; i < this->pImpl_Window->dimen.y * this->pImpl_Window->dimen.x; i++)
        {
            this->pImpl_Window->dirtyPoints[i] = this->pImpl_Window->bufferBaseDetail;
        }

        return;
    }

    Window::Window(PointDetail bufferBaseDetail)
    {
        this->pImpl_Window = nullptr;

        this->pImpl_Window = new Impl_Window();

        if (!this->pImpl_Window)
        {
            throw std::runtime_error("error init window");
        }

        this->pImpl_Window->o_handle = GetStdHandle(STD_OUTPUT_HANDLE);
        this->pImpl_Window->i_handle = GetStdHandle(STD_INPUT_HANDLE);

        set_special_console_flags_win32(this->pImpl_Window->i_handle);

        this->pImpl_Window->dimen = this->GetDimensions();

        this->pImpl_Window->bufferBaseDetail = bufferBaseDetail;

        this->pImpl_Window->command_buffer.reserve(4096);

        this->pImpl_Window->frameBuffer.resize(this->pImpl_Window->dimen.x * this->pImpl_Window->dimen.y);

        for (int i = 0; i < this->pImpl_Window->dimen.y * this->pImpl_Window->dimen.x; i++)
        {
            this->pImpl_Window->dirtyPoints[i] = bufferBaseDetail;
        }

        std::ios_base::sync_with_stdio(false);
        std::cin.tie(nullptr);

        this->Render();
    }

    void Window::SetTitle(std::string title)
    {
        this->pImpl_Window->title = title;
        SetConsoleTitleA(title.c_str());
    }

    const PointDetail &Window::GetBufferBaseDetail()
    {
        return this->pImpl_Window->bufferBaseDetail;
    }

    Vector2D Window::GetDimensions()
    {

        Vector2D dimen;

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(this->pImpl_Window->o_handle, &csbi);
        dimen.x = csbi.dwSize.X;
        dimen.y = csbi.dwSize.Y;
        return dimen;
    }

    void Window::Refresh()
    {
        Vector2D newDimen = this->GetDimensions();

        if (IsEqualVector2D(newDimen, this->pImpl_Window->dimen) == Logic::False)
        {
            this->pImpl_Window->dimen = newDimen;
            this->Retrace();
            set_special_console_flags_win32(this->pImpl_Window->i_handle);
        }
    }

    void Window::CMDSetCursorPosition(int x, int y)
    {

        std::string cmd = "\x1b[" + std::to_string(y + 1) + ";" +
                          std::to_string(x + 1) + "H";
        this->pImpl_Window->command_buffer += cmd;
        return;
    }

    void Window::SetCursorVisibleLogic(Logic logic)
    {

        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(this->pImpl_Window->o_handle, &info);
        info.bVisible = (logic == Logic::True ? TRUE : FALSE);
        SetConsoleCursorInfo(this->pImpl_Window->o_handle, &info);

        return;
    }

    void Window::CMDSetFGColor(Color color)
    {
        // this->pImpl_Window->fg_color = color;

        std::string cmd = std::string("\033[38;2;") + std::to_string(color.r) + ";" + std::to_string(color.g) + ";" + std::to_string(color.b) + "m";

        this->pImpl_Window->command_buffer += cmd;

        return;
    }

    void Window::CMDSetBGColor(Color color)
    {
        // this->pImpl_Window->bg_color = color;

        std::string cmd = std::string("\033[48;2;") + std::to_string(color.r) + ";" + std::to_string(color.g) + ";" + std::to_string(color.b) + "m";

        this->pImpl_Window->command_buffer += cmd;

        return;
    }

    void Window::SoftClearBuffer()
    {

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(this->pImpl_Window->o_handle, &csbi);

        COORD newSize;
        newSize.X = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        newSize.Y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        SetConsoleScreenBufferSize(this->pImpl_Window->o_handle, newSize);

        int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

        SetConsoleScreenBufferSize(this->pImpl_Window->o_handle, newSize);

        DWORD written;
        COORD topLeft = {0, 0};
        FillConsoleOutputCharacter(this->pImpl_Window->o_handle, ' ', newSize.X * newSize.Y, topLeft, &written);
        FillConsoleOutputAttribute(this->pImpl_Window->o_handle, csbi.wAttributes, newSize.X * newSize.Y, topLeft, &written);

        SetConsoleCursorPosition(this->pImpl_Window->o_handle, topLeft);
    }

    void Window::HardClearBuffer()
    {

        this->SoftClearBuffer();

        system("cls");
    }

    void Window::CMDSetOverloadStyle(OverloadStyle style)
    {

        this->pImpl_Window->command_buffer += OverloadStyleToANSI(style);
    }

    PointDetail Window::GetPointDetail(int position)
    {
        if (position < 0 || position >= (this->pImpl_Window->dimen.x * this->pImpl_Window->dimen.y))
            return MakePointDetail(' ', MakeStyle(MakeColor(0, 0, 0), MakeColor(0, 0, 0), OverloadStyle::None));

        return this->pImpl_Window->frameBuffer[position];
    }

    PointDetail Window::GetPointDetail(Vector2D position)
    {

        int pos = position.x + position.y * this->pImpl_Window->dimen.x;
        if (pos < 0 || pos >= (this->pImpl_Window->dimen.x * this->pImpl_Window->dimen.y))
            return MakePointDetail(' ', MakeStyle(MakeColor(0, 0, 0), MakeColor(0, 0, 0), OverloadStyle::None));

        return this->pImpl_Window->frameBuffer[pos];
    }

    void Window::CMDSetCharacter(char ch)
    {
        this->pImpl_Window->command_buffer += ch;
    }

    void Window::ClearStagedCMDs()
    {
        this->pImpl_Window->command_buffer.clear();
        this->pImpl_Window->dirtyPoints.clear();
    }

    void Window::CMDBlit()
    {
        std::cout << this->pImpl_Window->command_buffer;
        std::flush(std::cout);
    }

    void Window::Render(Logic enableDiffing)
    {

        PointDetail detail;
        int position;
        for (auto it : this->pImpl_Window->dirtyPoints)
        {

            position = it.first;
            detail = it.second;

            PointDetail &prevDetail = this->pImpl_Window->frameBuffer[position];

            if (enableDiffing == Logic::True)
            {
                if (IsEqualPointDetail(prevDetail, detail) == Logic::True)
                    continue;
            }

            // set the cursor position
            this->CMDSetCursorPosition(position % this->pImpl_Window->dimen.x, position / this->pImpl_Window->dimen.x);
            // command point bgColor
            this->CMDSetBGColor(detail.styles.bg_color);
            // command point fgColor
            this->CMDSetFGColor(detail.styles.fg_color);
            // additionalstyle
            this->CMDSetOverloadStyle(detail.styles.overloadStyle);
            // command point character
            this->CMDSetCharacter(detail.ch);

            // reset the bits
            // cmd of OVERLOAD_STYLE_RESET_BIT reset the any state of point detail no need to reset everything manually
            this->CMDSetOverloadStyle(OverloadStyle::ResetBit);

            this->pImpl_Window->frameBuffer[position] = detail;
        }

        // this->CMDSetBGColor(this->pImpl_Window->bufferBaseDetail.bg_color);
        // this->CMDSetFGColor(this->pImpl_Window->bufferBaseDetail.fg_color);
        // this->CMDSetOverloadStyle(this->pImpl_Window->bufferBaseDetail.style);

        // blit
        this->CMDBlit();

        // clear old stages
        this->ClearStagedCMDs();

        // this->pImpl_Window->command_buffer.clear();
        // this->pImpl_Window->dirtyPoints.clear();
    }

    void Window::StoreCommandBuffer(std::string filename, Logic overWrite)
    {
        std::ifstream f(filename, std::ios::in);

        if (!f.is_open())
        {
            std::ofstream file(filename);

            file << this->pImpl_Window->command_buffer;

            file.close();
        }
        else
        {
            if (overWrite == Logic::True)
            {
                std::ofstream file(filename);
                file << this->pImpl_Window->command_buffer;
                file.close();
            }
            else
            {
                std::cerr << "could not overWrite existing file" << std::endl;
                f.close();
                return;
            }
        }

        f.close();
    }

    void Window::RestoreConfig()
    {
        set_special_console_flags_win32(this->pImpl_Window->i_handle);
    }

    Window::~Window()
    {
        if (this->pImpl_Window)
        {
            delete this->pImpl_Window;
        }
    }
    // };
}
#endif