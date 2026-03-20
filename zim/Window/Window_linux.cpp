#ifndef WINDOW_CPP
#define WINDOW_CPP

#include "../Window.hpp"
#include "stdio.h"
#include "sys/termios.h"
#include "unistd.h"
#include "fcntl.h"
#include "sys/ioctl.h"
#include "signal.h"
#include "sys/types.h"

#include "iostream"
#include "fstream"
#include "unordered_map"
#include "queue"

struct termios original;
struct termios raw;

void enable_raw_mode(int fd)
{
    tcgetattr(fd, &original);
    raw = original;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(fd, TCSANOW, &raw);
}

void disable_raw_mode(int fd){
    tcsetattr(fd,TCSANOW,&original);
}

void enableMouseTracking()
{
    const char *seq = "\033[?1000h\033[?1003h\033[?1006h";
    write(STDOUT_FILENO, seq, 24);
}


zim::Vector2D linux_internal_get_window_size(int fd)
{
    struct winsize window_size;
    ioctl(fd, TIOCGWINSZ, &window_size);
    return zim::MakeVector2D(window_size.ws_col, window_size.ws_row);
}

void ResetEventImage(zim::EventImage &eventImage)
{
    // zim::EventImage &eventImage = this->pImpl_Window->event_image;

    eventImage.eventOccuredLogic = zim::Logic::False;

    eventImage.keyboardEvent.keys.MakeEmpty();

    eventImage.mouseEvent.keyPressed = zim::MouseKey::None;
    eventImage.mouseEvent.scroll = zim::MakeVector2D(0, 0);

    eventImage.windowEvent.isFocusedLogic = zim::Logic::True;
    eventImage.windowEvent.isResizedLogic = zim::Logic::False;
}

void CaptureInput(int fd, zim::EventImage &eventImage)
{

    auto decodeModifier = [](int terminalModCode) -> zim::KeyboardKey {
        // xterm modifier code is 1 + bitmask(Shift=1, Alt=2, Ctrl=4)
        const int bits = terminalModCode - 1;
        if (bits & 4)
            return zim::KeyboardKey::LeftCtrl;
        if (bits & 2)
            return zim::KeyboardKey::LeftAlt;
        if (bits & 1)
            return zim::KeyboardKey::Shift;
        return zim::KeyboardKey::None;
    };

    char buf[64] = {0};
    int n = read(fd, buf, sizeof(buf) - 1);
    ResetEventImage(eventImage);
    if (n <= 0)
        return;

    buf[n] = '\0';

    eventImage.eventOccuredLogic = zim::Logic::True;

    // --- Mouse SGR sequences ESC [ < BUTTON ; X ; Y M/m ---
    if (n >= 6 && buf[0] == 27 && buf[1] == '[' && buf[2] == '<')
    {
        int b, x, y;
        char action;
        if (sscanf(buf, "\033[<%d;%d;%d%c", &b, &x, &y, &action) == 4)
        {
            if (action == 'M')
            {
                if (b == 0)
                    eventImage.mouseEvent.keyPressed = zim::MouseKey::Left;
                else if (b == 1)
                    eventImage.mouseEvent.keyPressed = zim::MouseKey::Middle;
                else if (b == 2)
                    eventImage.mouseEvent.keyPressed = zim::MouseKey::Right;
                else if (b == 64)
                    eventImage.mouseEvent.scroll.y = 1;
                else if (b == 65)
                    eventImage.mouseEvent.scroll.y = -1;
                else if (b == 66)
                    eventImage.mouseEvent.scroll.x = -1;
                else if (b == 67)
                    eventImage.mouseEvent.scroll.x = 1;
            }
            eventImage.mouseEvent.position = zim::MakeVector2D(x-1, y-1);
            return;
        }
    }

    // --- Single-byte keys ---
    if (n == 1)
    {
        char c = buf[0];
        zim::KeyState state;

        // Handle non-printable control keys first so Enter is not decoded as Ctrl+J.
        if (c == 9)
        {
            state.key = zim::KeyboardKey::Tab;
            state.modifier = zim::KeyboardKey::None;
        }
        else if (c == 10 || c == 13)
        {
            state.key = zim::KeyboardKey::Enter;
            state.modifier = zim::KeyboardKey::None;
        }
        else if (c == 27)
        {
            state.key = zim::KeyboardKey::Escape;
            state.modifier = zim::KeyboardKey::None;
        }
        else if (c == 127)
        {
            state.key = zim::KeyboardKey::Backspace;
            state.modifier = zim::KeyboardKey::None;
        }
        // Ctrl + A-Z (except keys handled above)
        else if (c >= 1 && c <= 26)
        {
            state.key = static_cast<zim::KeyboardKey>('A' + c - 1);
            state.modifier = zim::KeyboardKey::LeftCtrl;
        }
        // Printable ASCII
        else if (c >= 32 && c <= 126)
        {
            state.key = static_cast<zim::KeyboardKey>(c);
            state.modifier = (c >= 'A' && c <= 'Z') ? zim::KeyboardKey::Shift : zim::KeyboardKey::None;
        }
        else
        {
            state.key = zim::KeyboardKey::Unknown;
            state.modifier = zim::KeyboardKey::None;
        }
        eventImage.keyboardEvent.keys.Put(state);
        return;
    }

    // --- Escape sequences (Alt, arrows, function keys, modifiers) ---
    if (buf[0] == 27)
    {
        zim::KeyState state;

        // Alt + key
        if (buf[1] != '[' && buf[1] != 'O')
        {
            state.key = static_cast<zim::KeyboardKey>(buf[1]);
            state.modifier = zim::KeyboardKey::LeftAlt;
            eventImage.keyboardEvent.keys.Put(state);
            return;
        }

        // CSI sequences ESC [ ...
        if (buf[1] == '[')
        {
            // Plain arrow keys: ESC [ A/B/C/D
            if (n >= 3)
            {
                bool matched = true;
                switch (buf[2])
                {
                case 'A':
                    state.key = zim::KeyboardKey::ArrowUp;
                    break;
                case 'B':
                    state.key = zim::KeyboardKey::ArrowDown;
                    break;
                case 'C':
                    state.key = zim::KeyboardKey::ArrowRight;
                    break;
                case 'D':
                    state.key = zim::KeyboardKey::ArrowLeft;
                    break;
                default:
                    matched = false;
                    break;
                }

                if (matched)
                {
                    state.modifier = zim::KeyboardKey::None;
                    eventImage.keyboardEvent.keys.Put(state);
                    return;
                }
            }

            // Arrow keys + modifier format: ESC [ 1 ; MOD CODE A/B/C/D
            if (n >= 6 && buf[2] == '1' && buf[3] == ';')
            {
                int mod;
                char dir;
                if (sscanf(buf, "\033[1;%d%c", &mod, &dir) == 2)
                {
                    switch (dir)
                    {
                    case 'A':
                        state.key = zim::KeyboardKey::ArrowUp;
                        break;
                    case 'B':
                        state.key = zim::KeyboardKey::ArrowDown;
                        break;
                    case 'C':
                        state.key = zim::KeyboardKey::ArrowRight;
                        break;
                    case 'D':
                        state.key = zim::KeyboardKey::ArrowLeft;
                        break;
                    default:
                        state.key = zim::KeyboardKey::Unknown;
                    }

                    state.modifier = decodeModifier(mod);

                    eventImage.keyboardEvent.keys.Put(state);
                    return;
                }
            }

            // Keys with modifiers in ~ form: ESC [ NUM ; MOD ~
            if (n >= 6)
            {
                int code;
                int mod;
                if (sscanf(buf, "\033[%d;%d~", &code, &mod) == 2)
                {
                    bool matched = true;
                    switch (code)
                    {
                    case 1:
                        state.key = zim::KeyboardKey::Home;
                        break;
                    case 2:
                        state.key = zim::KeyboardKey::Insert;
                        break;
                    case 3:
                        state.key = zim::KeyboardKey::Delete;
                        break;
                    case 4:
                        state.key = zim::KeyboardKey::End;
                        break;
                    case 5:
                        state.key = zim::KeyboardKey::PageUp;
                        break;
                    case 6:
                        state.key = zim::KeyboardKey::PageDown;
                        break;
                    case 15:
                        state.key = zim::KeyboardKey::F5;
                        break;
                    case 17:
                        state.key = zim::KeyboardKey::F6;
                        break;
                    case 18:
                        state.key = zim::KeyboardKey::F7;
                        break;
                    case 19:
                        state.key = zim::KeyboardKey::F8;
                        break;
                    case 20:
                        state.key = zim::KeyboardKey::F9;
                        break;
                    case 21:
                        state.key = zim::KeyboardKey::F10;
                        break;
                    case 23:
                        state.key = zim::KeyboardKey::F11;
                        break;
                    case 24:
                        state.key = zim::KeyboardKey::F12;
                        break;
                    default:
                        matched = false;
                        break;
                    }

                    if (matched)
                    {
                        state.modifier = decodeModifier(mod);
                        eventImage.keyboardEvent.keys.Put(state);
                        return;
                    }
                }
            }

            // Page Up / Page Down / Home / End / Insert / Delete / F5-F12
            if (n >= 5)
            {
                bool matched = false;
                if (buf[2] == '5' && buf[3] == '~')
                {
                    state.key = zim::KeyboardKey::PageUp;
                    state.modifier = zim::KeyboardKey::None;
                    matched = true;
                }
                else if (buf[2] == '6' && buf[3] == '~')
                {
                    state.key = zim::KeyboardKey::PageDown;
                    state.modifier = zim::KeyboardKey::None;
                    matched = true;
                }
                else if (buf[2] == '1' && buf[3] == '~')
                {
                    state.key = zim::KeyboardKey::Home;
                    state.modifier = zim::KeyboardKey::None;
                    matched = true;
                }
                else if (buf[2] == '4' && buf[3] == '~')
                {
                    state.key = zim::KeyboardKey::End;
                    state.modifier = zim::KeyboardKey::None;
                    matched = true;
                }
                else if (buf[2] == '2' && buf[3] == '~')
                {
                    state.key = zim::KeyboardKey::Insert;
                    state.modifier = zim::KeyboardKey::None;
                    matched = true;
                }
                else if (buf[2] == '3' && buf[3] == '~')
                {
                    state.key = zim::KeyboardKey::Delete;
                    state.modifier = zim::KeyboardKey::None;
                    matched = true;
                }
                else if (buf[2] == '1' && buf[3] == '5' && buf[4] == '~')
                {
                    state.key = zim::KeyboardKey::F5;
                    matched = true;
                }
                else if (buf[2] == '1' && buf[3] == '7' && buf[4] == '~')
                {
                    state.key = zim::KeyboardKey::F6;
                    matched = true;
                }
                else if (buf[2] == '1' && buf[3] == '8' && buf[4] == '~')
                {
                    state.key = zim::KeyboardKey::F7;
                    matched = true;
                }
                else if (buf[2] == '1' && buf[3] == '9' && buf[4] == '~')
                {
                    state.key = zim::KeyboardKey::F8;
                    matched = true;
                }
                else if (buf[2] == '2' && buf[3] == '0' && buf[4] == '~')
                {
                    state.key = zim::KeyboardKey::F9;
                    matched = true;
                }
                else if (buf[2] == '2' && buf[3] == '1' && buf[4] == '~')
                {
                    state.key = zim::KeyboardKey::F10;
                    matched = true;
                }
                else if (buf[2] == '2' && buf[3] == '3' && buf[4] == '~')
                {
                    state.key = zim::KeyboardKey::F11;
                    matched = true;
                }
                else if (buf[2] == '2' && buf[3] == '4' && buf[4] == '~')
                {
                    state.key = zim::KeyboardKey::F12;
                    matched = true;
                }

                if (matched)
                {
                    if (state.modifier != zim::KeyboardKey::LeftCtrl &&
                        state.modifier != zim::KeyboardKey::LeftAlt &&
                        state.modifier != zim::KeyboardKey::Shift)
                    {
                        state.modifier = zim::KeyboardKey::None;
                    }
                    eventImage.keyboardEvent.keys.Put(state);
                    return;
                }
            }
        }

        // Function keys F1-F4
        if (buf[1] == 'O')
        {
            switch (buf[2])
            {
            case 'P':
                state.key = zim::KeyboardKey::F1;
                break;
            case 'Q':
                state.key = zim::KeyboardKey::F2;
                break;
            case 'R':
                state.key = zim::KeyboardKey::F3;
                break;
            case 'S':
                state.key = zim::KeyboardKey::F4;
                break;
            default:
                state.key = zim::KeyboardKey::Unknown;
            }
            state.modifier = zim::KeyboardKey::None;
            eventImage.keyboardEvent.keys.Put(state);
            return;
        }
    }

}


void CaptureWindowEvent(int fd,zim::Vector2D& oldDimen,zim::EventImage& eventImage){
    zim::Vector2D newSize = linux_internal_get_window_size(fd);

        if(newSize.x!=oldDimen.x || newSize.y!=oldDimen.y){
            eventImage.windowEvent.isResizedLogic=zim::Logic::True;
        }

        pid_t fg_pid= tcgetpgrp(fd);
        pid_t my_pid = getpgrp();

        if(fg_pid==my_pid){
            eventImage.windowEvent.isFocusedLogic=zim::Logic::True;
        }else{
            eventImage.windowEvent.isFocusedLogic=zim::Logic::False;
        }
}

namespace zim
{
    struct Window::Impl_Window
    {
        std::string title;

        int fd;
        // struct termios terminal;

        Vector2D dimen;

        PointDetail bufferBaseDetail;

        std::vector<PointDetail> frameBuffer;

        EventImage event_image;

        std::string command_buffer;

        std::unordered_map<int, PointDetail> dirtyPoints;
    };

    void Window::CaptureEventImage()
    {
        ResetEventImage(this->pImpl_Window->event_image);

        CaptureInput(this->pImpl_Window->fd,this->pImpl_Window->event_image);

        CaptureWindowEvent(this->pImpl_Window->fd,this->pImpl_Window->dimen,this->pImpl_Window->event_image);
        
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

        this->pImpl_Window->fd = STDIN_FILENO;

        enable_raw_mode(this->pImpl_Window->fd);
        enableMouseTracking();


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
#if defined(_WIN32) || defined(_WIN64)
        SetConsoleTitleA(title.c_str());
#elif defined(__linux__)
        if (isatty(this->pImpl_Window->fd))
        {
            write(STDOUT_FILENO, "\033]0;", 4);
            write(STDOUT_FILENO, title.c_str(), title.length());
            write(STDOUT_FILENO, "\007", 1);
        }
#else
#error "Unsupported Platform"
#endif
    }

    const PointDetail &Window::GetBufferBaseDetail()
    {
        return this->pImpl_Window->bufferBaseDetail;
    }

    Vector2D Window::GetDimensions()
    {

        Vector2D dimen;

#if defined(_WIN32) || defined(_WIN64)
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(this->pImpl_Window->o_handle, &csbi);
        dimen.x = csbi.dwSize.X;
        dimen.y = csbi.dwSize.Y;
#elif defined(__linux__)

        struct winsize ws;
        if (ioctl(this->pImpl_Window->fd, TIOCGWINSZ, &ws) == 0)
        {
            dimen.x = ws.ws_col;
            dimen.y = ws.ws_row;
        }
        else
        {
            dimen.x = 0;
            dimen.y = 0;
        }

#else
#error "Unsupported Platform"
#endif

        return dimen;
    }

    void Window::Refresh()
    {
        Vector2D newDimen = this->GetDimensions();

        if (IsEqualVector2D(newDimen, this->pImpl_Window->dimen) == Logic::False)
        {
            this->pImpl_Window->dimen = newDimen;
            this->Retrace();
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

#if defined(_WIN32) || defined(_WIN64)
        CONSOLE_CURSOR_INFO info;
        GetConsoleCursorInfo(this->pImpl_Window->o_handle, &info);
        info.bVisible = (logic == Logic::True ? TRUE : FALSE);
        SetConsoleCursorInfo(this->pImpl_Window->o_handle, &info);
#elif defined(__linux__)

        std::string buf = (logic == Logic::True ? "\033[?25h" : "\033[?25l");

        write(STDOUT_FILENO, buf.c_str(), buf.length());

#else
#error "Unsupported Platform"
#endif

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

#if defined(_WIN32) || defined(_WIN64)

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

#elif defined(__linux__)
        std::string buf = "\033[2J\033[H";
        write(STDOUT_FILENO, buf.c_str(), buf.length());
#else
#error "Unsupported Platform"
#endif
    }

    void Window::HardClearBuffer()
    {

        this->SoftClearBuffer();

#if defined(_WIN32) || defined(_WIN64)
        system("cls");
#elif defined(__linux__)
        system("clear");
#else
#error "Unsupported Platform"
#endif
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
        tcsetattr(this->pImpl_Window->fd,TCSANOW,&raw);
    }

    Window::~Window()
    {
        if (this->pImpl_Window)
        {
            disable_raw_mode(this->pImpl_Window->fd);
            const char *seq = "\033[?1006l\033[?1003l\033[?1000l";
            write(STDOUT_FILENO, seq, 24);

            delete this->pImpl_Window;
        }
    }
}
#endif