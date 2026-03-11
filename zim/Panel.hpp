#pragma once

#ifndef PANEL_HPP
#define PANEL_HPP

#include "Types.hpp"
#include "string"

namespace zim
{

    class Window;

    class Panel
    {
        struct Impl_Panel;
        struct Impl_Panel *pImpl_Panel;

        void MarkBorderDirty();

    public:
        void RetraceBuffer();
        void RetraceBorders();
        void RefreshBufferDimensions();

        void BindWindowContext(Window &window);


        StringDetail &Title();

        // void SetPosition(Vector2D position);

        void SetBufferBGColor(Color color);

        void SetBufferFGColor(Color color);

        void Retrace();

        Panel(Vector2D position, Vector2D dimension);

        void Resize(Vector2D dimension);

        void SetPanelChar(char ch, int x, int y);

        const PointDetail GetPointDetail(int position);

        void SetPointDetail(Vector2D
                                position,
                            PointDetail detail);

        const PointDetail GetPointDetail(Vector2D position);

        void ClearBuffer();

        void ClearBuffer(PointDetail pointDetail);

        void WriteStr(const std::string &str, int x, int y, Logic wrap);

        void WriteStrDetailed(const std::string &str, Vector2D position, Logic wrapLogic, Style style);

        Vector2D &Position();

        Vector2D &Dimension();

        void SetBorderBGColor(Color color);

        const Color &GetBorderBGColor() const;

        const Color &GetBorderFGColor() const;

        void SetBorderFGColor(Color color);

        void SetTopBorderChar(char ch);

        char GetTopBorderChar() const;

        void SetBottomBorderChar(char ch);

        char GetBottomBorderChar() const;

        void SetLeftBorderChar(char ch);

        char GetLeftBorderChar() const;

        void SetRightBorderChar(char ch);

        char GetRightBorderChar() const;

        void SetPadding(int padding);

        int GetPadding() const;

        int GetBufferHeight() const;

        void SetBorderChars(char top, char bottom, char left, char right);

        int GetBufferWidth() const;

        ~Panel();
    };
}

#endif