#ifndef PANEL_CPP
#define PANEL_CPP

#include "Panel.hpp"
#include "Window.hpp"
#include "iostream"
#include "string"
#include "vector"

namespace zim
{
    struct Panel::Impl_Panel
    {

        StringDetail title;

        Vector2D position;
        Vector2D dimension;
        int padding;

        // char border_char;
        char top_border_char = '-';
        char bottom_border_char = '-';
        char left_border_char = '|';
        char right_border_char = '|';

        char top_left_corner_char = '+';
        char top_right_corner_char = '+';
        char bottom_left_corner_char = '+';
        char bottom_right_corner_char = '+';

        int buffer_height;
        int buffer_width;

        // std::string panel_buffer;

        std::vector<PointDetail> buffer;

        Color border_bg_color;
        Color border_fg_color;

        Color buffer_bg_color;
        Color buffer_fg_color;

        Window *window = nullptr;
    };

    StringDetail &Panel::Title()
    {
        return this->pImpl_Panel->title;
    }

    void Panel::MarkBorderDirty()
    {
        if (!this->pImpl_Panel->window)
            return;

        int width = this->pImpl_Panel->dimension.x;
        int height = this->pImpl_Panel->dimension.y;
        int pad = this->pImpl_Panel->padding;

        if (width <= 0 || height <= 0)
            return;

        std::string title_displayed;
        int width_available = width - 2;

        if (width_available < 0)
            width_available = 0;

        int title_len = this->pImpl_Panel->title.sentence.length();

        if (title_len <= width_available)
        {
            title_displayed = this->pImpl_Panel->title.sentence;
        }
        else
        {
            if (width_available >= 3)
            {
                int cut = width_available - 3;
                title_displayed =
                    this->pImpl_Panel->title.sentence.substr(0, cut) + "...";
            }
            else
            {
                title_displayed = this->pImpl_Panel->title.sentence.substr(0, width_available);
            }
        }

        // mark top and bottom borders
        for (int i = 0; i < pad; i++)
        {
            for (int j = 0; j < width; j++)
            {

                if (i == 0 && j > 0 && j < width - 1 && j - 1 < title_displayed.length())
                {
                    this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + j, this->pImpl_Panel->position.y + i), MakePointDetail(title_displayed[j - 1], MakeStyle(this->pImpl_Panel->title.styles.bg_color, this->pImpl_Panel->title.styles.fg_color, this->pImpl_Panel->title.styles.overloadStyle)));
                }
                else
                {

                    this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + j, this->pImpl_Panel->position.y + i), MakePointDetail(this->pImpl_Panel->top_border_char, MakeStyle(this->pImpl_Panel->border_bg_color, this->pImpl_Panel->border_fg_color, OverloadStyle::None)));
                }

                this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + j, this->pImpl_Panel->position.y + height - 1 - i), MakePointDetail(this->pImpl_Panel->bottom_border_char, MakeStyle(this->pImpl_Panel->border_bg_color, this->pImpl_Panel->border_fg_color, OverloadStyle::None)));
            }
        }

        if (this->pImpl_Panel->padding > 0)
        {
            this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + 0, this->pImpl_Panel->position.y + 0), MakePointDetail(this->pImpl_Panel->top_left_corner_char, MakeStyle(this->pImpl_Panel->border_bg_color, this->pImpl_Panel->border_fg_color, OverloadStyle::None)));

            this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + width - 1, this->pImpl_Panel->position.y + 0), MakePointDetail(this->pImpl_Panel->top_right_corner_char, MakeStyle(this->pImpl_Panel->border_bg_color, this->pImpl_Panel->border_fg_color, OverloadStyle::None)));

            this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + 0, this->pImpl_Panel->position.y + height - 1 - 0), MakePointDetail(this->pImpl_Panel->bottom_left_corner_char, MakeStyle(this->pImpl_Panel->border_bg_color, this->pImpl_Panel->border_fg_color, OverloadStyle::None)));

            this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + width - 1, this->pImpl_Panel->position.y + height - 1 - 0), MakePointDetail(this->pImpl_Panel->bottom_right_corner_char, MakeStyle(this->pImpl_Panel->border_bg_color, this->pImpl_Panel->border_fg_color, OverloadStyle::None)));
        }
        // mark left and right borders
        for (int i = pad; i < height - pad; i++)
        {
            for (int j = 0; j < pad; j++)
            {
                // this->pImpl_Panel->dirtyPositions.insert({j, i});

                this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + j, this->pImpl_Panel->position.y + i), MakePointDetail(this->pImpl_Panel->left_border_char, MakeStyle(this->pImpl_Panel->border_bg_color, this->pImpl_Panel->border_fg_color, OverloadStyle::None)));

                // this->pImpl_Panel->dirtyPositions.insert({width - 1 - j, i});

                this->pImpl_Panel->window->SetPointDetail(MakeVector2D(this->pImpl_Panel->position.x + width - 1 - j, this->pImpl_Panel->position.y + i), MakePointDetail(this->pImpl_Panel->right_border_char, MakeStyle(this->pImpl_Panel->border_bg_color, this->pImpl_Panel->border_fg_color, OverloadStyle::None)));
            }
        }
    }

    void Panel::SetBufferBGColor(Color color)
    {
        this->pImpl_Panel->buffer_bg_color = color;
        // this->RetraceBuffer();
    }

    void Panel::SetBufferFGColor(Color color)
    {
        this->pImpl_Panel->buffer_fg_color = color;
        // this->RetraceBuffer();
    }

    void Panel::RefreshBufferDimensions()
    {

        int old_buffer_height = this->GetBufferHeight();
        int old_buffer_width = this->GetBufferWidth();

        int new_buffer_height = this->pImpl_Panel->dimension.y - 2 * this->pImpl_Panel->padding;

        int new_buffer_width = this->pImpl_Panel->dimension.x - 2 * this->pImpl_Panel->padding;

        if (new_buffer_height < 0)
            new_buffer_height = 0;
        if (new_buffer_width < 0)
            new_buffer_width = 0;

        if (new_buffer_height != old_buffer_height || new_buffer_width != old_buffer_width)
        {

            // this->pImpl_Panel->panel_buffer.resize(new_buffer_height * new_buffer_width, ' ');

            this->pImpl_Panel->buffer.resize(new_buffer_width * new_buffer_height, MakePointDetail(' ', MakeStyle(this->pImpl_Panel->buffer_bg_color, this->pImpl_Panel->buffer_fg_color, OverloadStyle::None)));

            this->pImpl_Panel->buffer_height = new_buffer_height;
            this->pImpl_Panel->buffer_width = new_buffer_width;
        }
    }

    void Panel::RetraceBorders()
    {
        this->MarkBorderDirty();
    }

    void Panel::RetraceBuffer()
    {
        if (!this->pImpl_Panel->window)
            return;

        if (this->pImpl_Panel->buffer_width <= 0)
            return; // Avoid division by zero

        for (int i = 0; i < this->pImpl_Panel->buffer.size(); i++)
        {
            int x = i % this->pImpl_Panel->buffer_width;
            int y = i / this->pImpl_Panel->buffer_width;

            PointDetail &detail = this->pImpl_Panel->buffer[i];
            // char ch = this->pImpl_Panel->buffer[i].ch;

            this->pImpl_Panel->window->SetPointDetail(
                MakeVector2D(this->pImpl_Panel->position.x + this->pImpl_Panel->padding + x, this->pImpl_Panel->position.y + this->pImpl_Panel->padding + y),
                detail);
        }
    }

    void Panel::Retrace()
    {
        if (!this->pImpl_Panel)
        {
            throw std::runtime_error("error init Panel");
        }

        RefreshBufferDimensions();

        this->RetraceBuffer();
        this->RetraceBorders();
        // this->RetraceCursor();
    }

    Panel::Panel(Vector2D position, Vector2D dimension)
    {
        this->pImpl_Panel = nullptr;
        this->pImpl_Panel = new Impl_Panel();

        if (!this->pImpl_Panel)
        {
            throw std::runtime_error("error init Panel");
        }

        this->pImpl_Panel->window = nullptr;

        this->pImpl_Panel->position = position;
        this->pImpl_Panel->dimension = dimension;
        this->pImpl_Panel->padding = 1;

        this->pImpl_Panel->top_border_char = '-';
        this->pImpl_Panel->bottom_border_char = '-';
        this->pImpl_Panel->left_border_char = '|';
        this->pImpl_Panel->right_border_char = '|';

        this->pImpl_Panel->border_bg_color = MakeColor(0, 0, 0);
        this->pImpl_Panel->border_fg_color = MakeColor(128, 128, 128);

        this->pImpl_Panel->buffer_bg_color = MakeColor(0, 0, 0);
        this->pImpl_Panel->buffer_fg_color = MakeColor(255, 255, 255);

        this->pImpl_Panel->title.styles.bg_color = MakeColor(255, 255, 255);
        this->pImpl_Panel->title.styles.fg_color = MakeColor(0, 0, 0);

        RefreshBufferDimensions();
    }

    void Panel::BindWindowContext(Window &window)
    {
        this->pImpl_Panel->window = &window;
    }

    void Panel::Resize(Vector2D dimension)
    {

        if (dimension.x < 0)
            dimension.x = 0;
        if (dimension.y < 0)
            dimension.y = 0;

        this->pImpl_Panel->dimension = dimension;
    }

    void Panel::SetPanelChar(char ch, int x, int y)
    {

        // Ensure coordinates are inside the client area
        if (x < 0 || x >= this->pImpl_Panel->buffer_width)
            return;
        if (y < 0 || y >= this->pImpl_Panel->buffer_height)
            return;

        // Linear index
        int idx = y * this->pImpl_Panel->buffer_width + x;

        PointDetail &detail = this->pImpl_Panel->buffer[idx];
        detail.ch = ch;
        // if (detail.ch != ch)
        // {
        // }
    }

    void Panel::SetPointDetail(Vector2D position, PointDetail detail)
    {

        if (position.x >= this->GetBufferWidth())
            return;

        int idx = position.y * this->GetBufferWidth() + position.x;

        if (idx < 0 || idx >= this->pImpl_Panel->buffer.size())
            return;

        PointDetail &innerDetail = this->pImpl_Panel->buffer[idx];

        this->pImpl_Panel->buffer[idx] = detail;
    }

    const PointDetail Panel::GetPointDetail(int position)
    {
        if (position < 0 || position >= this->pImpl_Panel->buffer.size())
        {
            return MakePointDetail(' ', MakeStyle(this->pImpl_Panel->buffer_bg_color, this->pImpl_Panel->buffer_fg_color, OverloadStyle::None));
        }
        return this->pImpl_Panel->buffer[position];
    }

    const PointDetail Panel::GetPointDetail(Vector2D position)
    {
        int idx = position.y * this->GetBufferWidth() + position.x;
        return this->GetPointDetail(idx);
    }

    void Panel::ClearBuffer()
    {

        Vector2D position;
        int buffer_width = this->GetBufferWidth();

        for (int i = 0; i < this->pImpl_Panel->buffer.size(); i++)
        {

            PointDetail &detail = this->pImpl_Panel->buffer[i];

            detail.ch = ' ';
            detail.styles = MakeStyle(this->pImpl_Panel->buffer_bg_color, this->pImpl_Panel->buffer_fg_color, OverloadStyle::None);
        }
    }

    void Panel::ClearBuffer(PointDetail pointDetail)
    {
        Vector2D position;
        int buffer_width = this->GetBufferWidth();

        for (int i = 0; i < this->pImpl_Panel->buffer.size(); i++)
        {

            PointDetail &actualDetailPoint = this->pImpl_Panel->buffer[i];

            actualDetailPoint = pointDetail;
        }
    }

    void Panel::WriteStr(const std::string &str, int x, int y, Logic wrap)
    {
        int x_pos = x;
        int y_pos = y;

        Style defaultStyle = MakeStyle(this->pImpl_Panel->buffer_bg_color,
                                       this->pImpl_Panel->buffer_fg_color,
                                       OverloadStyle::None);

        for (char ch : str)
        {
            if (y_pos >= this->pImpl_Panel->buffer_height)
                break;

            this->SetPointDetail(MakeVector2D(x_pos, y_pos), MakePointDetail(ch, defaultStyle));

            x_pos++;

            if (wrap == Logic::True && x_pos >= this->pImpl_Panel->buffer_width)
            {
                x_pos = 0; // start of next line
                y_pos++;
            }
        }
    }
    void Panel::WriteStrDetailed(const std::string &str, Vector2D position, Logic wrapLogic, Style style)
    {
        int x_pos = position.x;
        int y_pos = position.y;

        for (char ch : str)
        {
            if (y_pos >= this->pImpl_Panel->buffer_height)
                break;

            this->SetPointDetail(MakeVector2D(x_pos, y_pos), MakePointDetail(ch, style));

            x_pos++;

            if (wrapLogic == Logic::True && x_pos >= this->pImpl_Panel->buffer_width)
            {
                x_pos = 0; // start of next line
                y_pos++;
            }
        }
    }

    Vector2D &Panel::Position()
    {
        return this->pImpl_Panel->position;
    }

    Vector2D &Panel::Dimension()
    {
        return this->pImpl_Panel->dimension;
    }

    void Panel::SetBorderBGColor(Color color)
    {
        Color &oldColor = this->pImpl_Panel->border_bg_color;

        // if (oldColor.r != color.r || oldColor.g != color.g || oldColor.b != color.b)
        // {
        oldColor = color;

        // this->MarkBorderDirty();
        // }
    }

    const Color &Panel::GetBorderBGColor() const
    {
        return this->pImpl_Panel->border_bg_color;
    }

    const Color &Panel::GetBorderFGColor() const
    {
        return this->pImpl_Panel->border_fg_color;
    }

    void Panel::SetBorderFGColor(Color color)
    {

        Color &oldColor = this->pImpl_Panel->border_fg_color;

        // if (oldColor.r != color.r || oldColor.g != color.g || oldColor.b != color.b)
        // {
        oldColor = color;
        // this->MarkBorderDirty();
        // }
    }

    void Panel::SetBorderChars(char top, char bottom, char left, char right)
    {
        // bool changed = false;

        this->pImpl_Panel->top_border_char = top;
        this->pImpl_Panel->bottom_border_char = bottom;
        this->pImpl_Panel->left_border_char = left;
        this->pImpl_Panel->right_border_char = right;

        // if (this->pImpl_Panel->top_border_char != top)
        // {
        //     changed = true;
        // }
        // if (this->pImpl_Panel->bottom_border_char != bottom)
        // {
        //     changed = true;
        // }
        // if (this->pImpl_Panel->left_border_char != left)
        // {
        //     changed = true;
        // }
        // if (this->pImpl_Panel->right_border_char != right)
        // {
        // changed = true;
        // }

        // if (changed)
        // {
        // this->MarkBorderDirty();
        // }
    }

    void Panel::SetTopBorderChar(char ch)
    {
        this->pImpl_Panel->top_border_char = ch;
        // if (this->pImpl_Panel->top_border_char != ch)
        // {
        //     // this->MarkBorderDirty();
        // }
    }

    char Panel::GetTopBorderChar() const
    {
        return this->pImpl_Panel->top_border_char;
    }

    void Panel::SetBottomBorderChar(char ch)
    {
        this->pImpl_Panel->bottom_border_char = ch;
        // if (this->pImpl_Panel->bottom_border_char != ch)
        // {
        //     // this->MarkBorderDirty();
        // }
    }

    char Panel::GetBottomBorderChar() const
    {
        return this->pImpl_Panel->bottom_border_char;
    }

    void Panel::SetLeftBorderChar(char ch)
    {
        this->pImpl_Panel->left_border_char = ch;
        // if (this->pImpl_Panel->left_border_char != ch)
        // {
        // this->MarkBorderDirty();
        // }
    }

    char Panel::GetLeftBorderChar() const
    {
        return this->pImpl_Panel->left_border_char;
    }

    void Panel::SetRightBorderChar(char ch)
    {
        this->pImpl_Panel->right_border_char = ch;
        // if (this->pImpl_Panel->right_border_char != ch)
        // {
        // this->MarkBorderDirty();
        // }
    }

    char Panel::GetRightBorderChar() const
    {
        return this->pImpl_Panel->right_border_char;
    }

    void Panel::SetPadding(int padding)
    {
        this->pImpl_Panel->padding = padding;
        // if (this->pImpl_Panel->padding != padding)
        // {
        //     // this->MarkBorderDirty();
        // }
    }

    int Panel::GetPadding() const
    {
        return this->pImpl_Panel->padding;
    }

    int Panel::GetBufferHeight() const
    {
        return this->pImpl_Panel->buffer_height;
    }

    int Panel::GetBufferWidth() const
    {
        return this->pImpl_Panel->buffer_width;
    }

    Panel::~Panel()
    {
        if (this->pImpl_Panel)
        {
            delete this->pImpl_Panel;
        }
    }
}
#endif