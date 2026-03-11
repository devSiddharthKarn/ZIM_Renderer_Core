#include "Types.hpp"
#include "fstream"

namespace zim
{

    Vector2D MakeVector2D(int x, int y)
    {
        Vector2D vector;
        vector.x = x;
        vector.y = y;
        return vector;
    }

    Logic IsEqualVector2D(Vector2D vec1, Vector2D vec2)
    {
        if (vec1.x != vec2.x || vec1.y != vec2.y)
            return Logic::False;
        return Logic::True;
    }

    Color MakeColor(int r, int g, int b)
    {
        Color color;
        color.r = r;
        color.g = g;
        color.b = b;

        return color;
    }

    Logic IsEqualColor(Color color1, Color color2)
    {
        if (color1.b != color2.b || color1.g != color2.g || color1.r != color2.r)
            return Logic::False;
        return Logic::True;
    }

    Style MakeStyle(Color bg_color, Color fg_color, OverloadStyle overloadStyle)
    {
        Style style;
        style.bg_color = bg_color;
        style.fg_color = fg_color;
        style.overloadStyle = overloadStyle;

        return style;
    }

    Logic IsEqualStyle(Style style1, Style style2)
    {
        if (IsEqualColor(style1.bg_color, style2.bg_color) == Logic::False || IsEqualColor(style1.fg_color, style2.fg_color) == Logic::False || style1.overloadStyle != style2.overloadStyle)
            return Logic::False;
        return Logic::True;
    }

    StringDetail MakeStringDetail(std::string sentence, Style style)
    {
        StringDetail detail;
        detail.sentence = sentence;
        detail.styles = style;
        return detail;
    }

    Logic IsEqualStringDetail(StringDetail detail1, StringDetail detail2)
    {
        if (detail1.sentence != detail2.sentence || IsEqualStyle(detail1.styles, detail2.styles) == Logic::False)
            return Logic::False;
        return Logic::True;
    }

    PointDetail MakePointDetail(char ch, Style style)
    {
        PointDetail detail;
        detail.ch = ch;
        detail.styles = style;
        return detail;
    }

    Logic IsEqualPointDetail(PointDetail detail1, PointDetail detail2)
    {
        if (detail1.ch != detail2.ch || IsEqualStyle(detail1.styles, detail2.styles) == Logic::False)
            return Logic::False;
        return Logic::True;
    }

    OverloadStyle operator&(OverloadStyle a, OverloadStyle b)
    {
        return static_cast<OverloadStyle>(static_cast<unsigned long long>(a) & static_cast<unsigned long long>(b));
    }

    OverloadStyle operator|(OverloadStyle a, OverloadStyle b)
    {
        return static_cast<OverloadStyle>(static_cast<unsigned long long>(a) | static_cast<unsigned long long>(b));
    }


    OverloadStyle operator^(OverloadStyle a, OverloadStyle b)
    {
        return static_cast<OverloadStyle>(static_cast<unsigned long long>(a) ^ static_cast<unsigned long long>(b));
    }

    OverloadStyle operator|=(OverloadStyle &a, OverloadStyle b)
    {
        a = static_cast<OverloadStyle>(static_cast<unsigned long long>(a) | static_cast<unsigned long long>(b));
        return a;
    }

    OverloadStyle operator&=(OverloadStyle &a, OverloadStyle b)
    {
        a = static_cast<OverloadStyle>(static_cast<unsigned long long>(a) & static_cast<unsigned long long>(b));
        return a;
    }

    OverloadStyle operator^=(OverloadStyle &a, OverloadStyle b)
    {
        a = static_cast<OverloadStyle>(static_cast<unsigned long long>(a) ^ static_cast<unsigned long long>(b));
        return a;
    }

    OverloadStyle operator<<(OverloadStyle a, int shift)
    {
        return static_cast<OverloadStyle>(static_cast<unsigned long long>(a) << shift);
    }

    OverloadStyle operator>>(OverloadStyle a, int shift)
    {
        return static_cast<OverloadStyle>(static_cast<unsigned long long>(a) >> shift);
    }

    OverloadStyle operator<<=(OverloadStyle &a, int shift)
    {
        a = static_cast<OverloadStyle>(static_cast<unsigned long long>(a) << shift);
        return a;
    }

    OverloadStyle operator>>=(OverloadStyle &a, int shift)
    {
        a = static_cast<OverloadStyle>(static_cast<unsigned long long>(a) >> shift);
        return a;
    }

    OverloadStyle operator~(OverloadStyle a)
    {
        return static_cast<OverloadStyle>(~static_cast<unsigned long long>(a));
    }

    Logic operator!(OverloadStyle a)
    {
        return static_cast<Logic>(!static_cast<unsigned long long>(a));
    }

    std::string OverloadStyleToANSI(OverloadStyle style)
    {

        if (style == OverloadStyle::None)
            return "";

        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::ResetBit))
            return "\x1b[0m";

        std::string out;
        out.reserve(64);
        out += "\x1b[";

        bool first = true;
        auto append = [&](int code)
        {
            if (!first)
                out += ';';
            first = false;
            out += std::to_string(code);
        };

        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Bold))
            append(1);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Dim))
            append(2);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Italic))
            append(3);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Underline))
            append(4);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::BlinkSlow))
            append(5);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::BlinkFast))
            append(6);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Reverse))
            append(7);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Hidden))
            append(8);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Strikethrough))
            append(9);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Framed))
            append(51);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Encircled))
            append(52);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Overline))
            append(53);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Superscript))
            append(73);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Subscript))
            append(74);

        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::NormalIntensity))
            append(22);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::NotItalic))
            append(23);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::UnderlineOff))
            append(24);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::BlinkOff))
            append(25);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::ReverseOff))
            append(27);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::Visible))
            append(28);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::StrikethroughOff))
            append(29);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::FramedOff))
            append(54);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::EncircledOff))
            append(54);
        if (static_cast<unsigned long long>(style) & static_cast<unsigned long long>(OverloadStyle::OverlineOff))
            append(55);

        out += 'm';
        return out;
    }
}
