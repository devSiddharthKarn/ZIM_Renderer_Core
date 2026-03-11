

#ifndef TYPES_HPP
#define TYPES_HPP

#pragma once

#include "string"
#include <stdexcept>

namespace zim
{

    enum class Logic : unsigned char
    {
        False = 0,
        True = 1
    };

    typedef struct
    {
        int x = 0;
        int y = 0;
    } Vector2D;

    Vector2D MakeVector2D(int x, int y);

    Logic IsEqualVector2D(Vector2D vec1, Vector2D vec2);

    struct Color
    {
        int r = 0;
        int g = 0;
        int b = 0;
    };

    Color MakeColor(int r, int g, int b);

    Logic IsEqualColor(Color color1, Color color2);

    enum class OverloadStyle : unsigned long long
    {
        None = 0,

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

    OverloadStyle operator|(OverloadStyle a, OverloadStyle b);

    OverloadStyle operator&(OverloadStyle a, OverloadStyle b);

    OverloadStyle operator~(OverloadStyle a);

    OverloadStyle operator^ (OverloadStyle a, OverloadStyle b);

    OverloadStyle operator|= (OverloadStyle &a, OverloadStyle b);

    OverloadStyle operator&= (OverloadStyle &a, OverloadStyle b);

    OverloadStyle operator^= (OverloadStyle &a, OverloadStyle b);

    OverloadStyle operator<< (OverloadStyle a, int shift);

    OverloadStyle operator>> (OverloadStyle a, int shift);

    OverloadStyle operator<<= (OverloadStyle &a, int shift);

    OverloadStyle operator>>= (OverloadStyle &a, int shift);

    OverloadStyle operator~(OverloadStyle a);

    Logic operator! (OverloadStyle a);


    typedef struct
    {
        OverloadStyle overloadStyle;
        Color bg_color;
        Color fg_color;
    } Style;

    Style MakeStyle(Color bg_color, Color fg_color, OverloadStyle overloadStyle = OverloadStyle::None);

    Logic IsEqualStyle(Style style1, Style style2);

    typedef struct
    {
        std::string sentence;
        Style styles;
    } StringDetail;

    StringDetail MakeStringDetail(std::string sentence, Style style);

    Logic IsEqualStringDetail(StringDetail detail1, StringDetail detail2);

    typedef struct
    {
        char ch;
        Style styles;
    } PointDetail;

    PointDetail MakePointDetail(char ch, Style style);

    Logic IsEqualPointDetail(PointDetail detail1, PointDetail detail2);

    std::string OverloadStyleToANSI(OverloadStyle style);

}
#endif
