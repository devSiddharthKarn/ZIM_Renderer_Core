#ifndef INPUT_HPP
#define INPUT_HPP

#pragma once

#include "Types.hpp"

namespace zim
{

    enum class MouseKey : int
    {
        None = 0,
        Left = (1ULL << 0),
        Right = (1ULL << 1),
        Middle = (1ULL << 2)
    };
    struct MouseEvent
    {
        MouseKey keyPressed;
        Vector2D scroll;
        Vector2D position;
    };

    enum class KeyboardKey : int
    {
        // ---------- No key ----------
        None = 0,

        // ---------- Control keys ----------
        Backspace = 8,
        Tab = 9,
        Enter = 13,
        Escape = 27,
        Space = 32,

        // ---------- Letters ----------
        A = 'A',
        B = 'B',
        C = 'C',
        D = 'D',
        E = 'E',
        F = 'F',
        G = 'G',
        H = 'H',
        I = 'I',
        J = 'J',
        K = 'K',
        L = 'L',
        M = 'M',
        N = 'N',
        O = 'O',
        P = 'P',
        Q = 'Q',
        R = 'R',
        S = 'S',
        T = 'T',
        U = 'U',
        V = 'V',
        W = 'W',
        X = 'X',
        Y = 'Y',
        Z = 'Z',

        a = 'a',
        b = 'b',
        c = 'c',
        d = 'd',
        e = 'e',
        f = 'f',
        g = 'g',
        h = 'h',
        i = 'i',
        j = 'j',
        k = 'k',
        l = 'l',
        m = 'm',
        n = 'n',
        o = 'o',
        p = 'p',
        q = 'q',
        r = 'r',
        s = 's',
        t = 't',
        u = 'u',
        v = 'v',
        w = 'w',
        x = 'x',
        y = 'y',
        z = 'z',

        SpecialSignal = 224,

        // ---------- Special keys (224 / 0 prefixed) ----------
        ArrowUp = 1000,
        ArrowDown,
        ArrowLeft,
        ArrowRight,

        Insert,
        Delete,
        Home,
        End,
        PageUp,
        PageDown,

        // ---------- Function keys ----------
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        // ---------- Unknown ----------
        Unknown,

        LeftCtrl,
        RightCtrl,
        Shift,
        LeftAlt,
        RightAlt,
        Capslock_on,
        Numlock_on,
        Scrolllock_on,

    };

    struct KeyState
    {
        KeyboardKey key;
        KeyboardKey modifier;

        KeyState(KeyboardKey key, KeyboardKey modifier);

        KeyState();

        void Reset();
    };

    class KeyQueue
    {
    private:
        struct Impl_KeyQueue;
        struct Impl_KeyQueue *pImpl_KeyQueue;

    public:
        KeyQueue();

        KeyState Get();

        void Put(KeyState key);

        void MakeEmpty();

        Logic IsEmpty() const;

        ~KeyQueue();
    };

    struct KeyboardEvent
    {
    private:
    public:
        KeyQueue keys;
        KeyboardEvent();
    };

    struct WindowEvent
    {
        Logic isResizedLogic;
        Logic isFocusedLogic;
    };

    struct EventImage
    {
        Logic eventOccuredLogic;
        WindowEvent windowEvent;
        KeyboardEvent keyboardEvent;
        MouseEvent mouseEvent;
    };
}
#endif
