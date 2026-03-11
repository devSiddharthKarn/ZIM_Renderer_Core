
#ifndef WINDOW_HPP
#define WINDOW_HPP

#pragma once

#include "Types.hpp"
#include "string"
#include "Input.hpp"

namespace zim
{

    class Panel;

    class Window
    {
        struct Impl_Window;
        struct Impl_Window *pImpl_Window;

        void CMDSetCursorPosition(int x, int y);
        void CMDSetOverloadStyle(OverloadStyle style);
        void CMDSetBGColor(Color color);
        void CMDSetFGColor(Color color);
        void CMDSetCharacter(char ch);
        void CMDBlit();
        void ClearStagedCMDs();

    public:
        void CaptureEventImage();

        EventImage &GetEventImage() const;

        const PointDetail &GetBufferBaseDetail();

        Window(PointDetail bufferBaseDetail);

        void Retrace();

        void SetPointDetail(int position, PointDetail detail);

        void SetPointDetail(Vector2D position, PointDetail detail);

        void SetTitle(std::string title);

        Vector2D GetDimensions();

        void Refresh();

        void SetCursorVisibleLogic(Logic logic);

        void HardClearBuffer();

        void SoftClearBuffer();

        PointDetail GetPointDetail(int position);

        PointDetail GetPointDetail(Vector2D position);

        void Render(Logic enableDiffing = Logic::True);

        void StoreCommandBuffer(std::string filename, Logic overWrite = Logic::False);

        ~Window();
    };

}
#endif