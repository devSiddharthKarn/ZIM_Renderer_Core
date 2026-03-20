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
    raw.c_cc[VMIN]=0;
    raw.c_cc[VTIME]=0;
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(fd, TCSANOW, &raw);

    // int flags = fcntl(fd,F_GETFL,0);
    // fcntl(fd,F_SETFL,flags|O_NONBLOCK);
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
        // xterm modifier code: 1 (none), 2 (shift), 3 (alt), 5 (ctrl).
        // Combination/unknown modifier codes map to Unknown.
        switch (terminalModCode)
        {
        case 1:
            return zim::KeyboardKey::None;
        case 2:
            return zim::KeyboardKey::Shift;
        case 3:
            return zim::KeyboardKey::LeftAlt;
        case 5:
            return zim::KeyboardKey::LeftCtrl;
        default:
            return zim::KeyboardKey::Unknown;
        }
    };

    auto decodeKeyCode = [](int code, zim::KeyboardKey &outKey) -> bool {
        switch (code)
        {
        case 1:
            outKey = zim::KeyboardKey::Home;
            return true;
        case 2:
            outKey = zim::KeyboardKey::Insert;
            return true;
        case 3:
            outKey = zim::KeyboardKey::Delete;
            return true;
        case 4:
            outKey = zim::KeyboardKey::End;
            return true;
        case 5:
            outKey = zim::KeyboardKey::PageUp;
            return true;
        case 6:
            outKey = zim::KeyboardKey::PageDown;
            return true;
        case 15:
            outKey = zim::KeyboardKey::F5;
            return true;
        case 17:
            outKey = zim::KeyboardKey::F6;
            return true;
        case 18:
            outKey = zim::KeyboardKey::F7;
            return true;
        case 19:
            outKey = zim::KeyboardKey::F8;
            return true;
        case 20:
            outKey = zim::KeyboardKey::F9;
            return true;
        case 21:
            outKey = zim::KeyboardKey::F10;
            return true;
        case 23:
            outKey = zim::KeyboardKey::F11;
            return true;
        case 24:
            outKey = zim::KeyboardKey::F12;
            return true;
        default:
            outKey = zim::KeyboardKey::Unknown;
            return false;
        }
    };

    char buf[256] = {0};
    int n = read(fd, buf, sizeof(buf) - 1);
    // ResetEventImage(eventImage);
    if (n <= 0)
    {
        return;
    }

    buf[n] = '\0';
    eventImage.eventOccuredLogic = zim::Logic::True;

    auto pushKey = [&eventImage](zim::KeyboardKey key, zim::KeyboardKey modifier) {
        eventImage.keyboardEvent.keys.Put(zim::KeyState(key, modifier));
    };

    auto parseSingleByte = [&pushKey](unsigned char c) {
        if (c == 9)
        {
            pushKey(zim::KeyboardKey::Tab, zim::KeyboardKey::None);
            return;
        }
        if (c == 10 || c == 13)
        {
            pushKey(zim::KeyboardKey::Enter, zim::KeyboardKey::None);
            return;
        }
        if (c == 127)
        {
            pushKey(zim::KeyboardKey::Backspace, zim::KeyboardKey::None);
            return;
        }
        if (c >= 1 && c <= 26)
        {
            pushKey(static_cast<zim::KeyboardKey>('a' + c - 1), zim::KeyboardKey::LeftCtrl);
            return;
        }
        if (c >= 32 && c <= 126)
        {
            pushKey(static_cast<zim::KeyboardKey>(c),
                    (c >= 'A' && c <= 'Z') ? zim::KeyboardKey::Shift : zim::KeyboardKey::None);
            return;
        }
        pushKey(zim::KeyboardKey::Unknown, zim::KeyboardKey::Unknown);
    };

    int i = 0;
    while (i < n)
    {
        unsigned char c = static_cast<unsigned char>(buf[i]);

        if (c != 27)
        {
            parseSingleByte(c);
            i++;
            continue;
        }

        const char *p = &buf[i];
        int consumed = 0;

        // Standalone Escape
        if (i + 1 >= n)
        {
            pushKey(zim::KeyboardKey::Escape, zim::KeyboardKey::None);
            i++;
            continue;
        }

        // Mouse SGR sequences: ESC [ < BUTTON ; X ; Y M/m
        int b = 0;
        int x = 0;
        int y = 0;
        char action = 0;
        if (sscanf(p, "\033[<%d;%d;%d%c%n", &b, &x, &y, &action, &consumed) == 4 && consumed > 0)
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
            eventImage.mouseEvent.position = zim::MakeVector2D(x - 1, y - 1);
            i += consumed;
            continue;
        }

        // Alt + key: ESC then non-CSI/non-SS3 byte
        if (buf[i + 1] != '[' && buf[i + 1] != 'O')
        {
            unsigned char altChar = static_cast<unsigned char>(buf[i + 1]);
            if (altChar >= 32 && altChar <= 126)
            {
                pushKey(static_cast<zim::KeyboardKey>(altChar), zim::KeyboardKey::LeftAlt);
            }
            else
            {
                pushKey(zim::KeyboardKey::Unknown, zim::KeyboardKey::LeftAlt);
            }
            i += 2;
            continue;
        }

        // CSI arrows with optional modifier: ESC [ 1 ; MOD A/B/C/D
        int mod = 0;
        char dir = 0;
        if (sscanf(p, "\033[1;%d%c%n", &mod, &dir, &consumed) == 2 && consumed > 0)
        {
            zim::KeyboardKey key = zim::KeyboardKey::Unknown;
            switch (dir)
            {
            case 'A':
                key = zim::KeyboardKey::ArrowUp;
                break;
            case 'B':
                key = zim::KeyboardKey::ArrowDown;
                break;
            case 'C':
                key = zim::KeyboardKey::ArrowRight;
                break;
            case 'D':
                key = zim::KeyboardKey::ArrowLeft;
                break;
            default:
                key = zim::KeyboardKey::Unknown;
                break;
            }
            pushKey(key, decodeModifier(mod));
            i += consumed;
            continue;
        }

        // CSI keys with modifier: ESC [ NUM ; MOD ~
        int code = 0;
        if (sscanf(p, "\033[%d;%d~%n", &code, &mod, &consumed) == 2 && consumed > 0)
        {
            zim::KeyboardKey key = zim::KeyboardKey::Unknown;
            if (decodeKeyCode(code, key))
            {
                pushKey(key, decodeModifier(mod));
            }
            else
            {
                pushKey(zim::KeyboardKey::Unknown, decodeModifier(mod));
            }
            i += consumed;
            continue;
        }

        // CSI plain arrows: ESC [ A/B/C/D
        if (sscanf(p, "\033[%c%n", &dir, &consumed) == 1 && consumed == 3)
        {
            zim::KeyboardKey key = zim::KeyboardKey::Unknown;
            switch (dir)
            {
            case 'A':
                key = zim::KeyboardKey::ArrowUp;
                break;
            case 'B':
                key = zim::KeyboardKey::ArrowDown;
                break;
            case 'C':
                key = zim::KeyboardKey::ArrowRight;
                break;
            case 'D':
                key = zim::KeyboardKey::ArrowLeft;
                break;
            default:
                break;
            }
            if (key != zim::KeyboardKey::Unknown)
            {
                pushKey(key, zim::KeyboardKey::None);
                i += consumed;
                continue;
            }
        }

        // CSI plain keys: ESC [ NUM ~
        if (sscanf(p, "\033[%d~%n", &code, &consumed) == 1 && consumed > 0)
        {
            zim::KeyboardKey key = zim::KeyboardKey::Unknown;
            if (decodeKeyCode(code, key))
            {
                pushKey(key, zim::KeyboardKey::None);
            }
            else
            {
                pushKey(zim::KeyboardKey::Unknown, zim::KeyboardKey::None);
            }
            i += consumed;
            continue;
        }

        // SS3 function keys: ESC O P/Q/R/S => F1/F2/F3/F4
        char fkey = 0;
        if (sscanf(p, "\033O%c%n", &fkey, &consumed) == 1 && consumed == 3)
        {
            switch (fkey)
            {
            case 'P':
                pushKey(zim::KeyboardKey::F1, zim::KeyboardKey::None);
                break;
            case 'Q':
                pushKey(zim::KeyboardKey::F2, zim::KeyboardKey::None);
                break;
            case 'R':
                pushKey(zim::KeyboardKey::F3, zim::KeyboardKey::None);
                break;
            case 'S':
                pushKey(zim::KeyboardKey::F4, zim::KeyboardKey::None);
                break;
            default:
                pushKey(zim::KeyboardKey::Unknown, zim::KeyboardKey::Unknown);
                break;
            }
            i += consumed;
            continue;
        }

        // Unrecognized escape sequence.
        pushKey(zim::KeyboardKey::Unknown, zim::KeyboardKey::Unknown);
        i++;
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
        std::string buf = "\033[2J\033[H";
        write(STDOUT_FILENO, buf.c_str(), buf.length());
    }

    void Window::HardClearBuffer()
    {

        this->SoftClearBuffer();

        system("clear");
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