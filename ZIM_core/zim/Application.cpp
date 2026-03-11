#include "Application.hpp"
#include "vector"
#include "unordered_map"

namespace zim
{

    struct Element::Impl_Element
    {
        Panel panel;
        std::vector<Element *> children;
        ElementLayoutHandler layoutHandler;
        Logic active;
        std::string id = "";

        EventListener eventListener;

        Impl_Element() : panel(MakeVector2D(0, 0), MakeVector2D(0, 0))
        {
            this->active = Logic::True;

            this->layoutHandler = [](Element &parent, Element &This, Element &document) {

            };

            this->eventListener = [](EventImage &eventImage, Element &This, Element &document) {};
        }
    };

    Element::Element()
    {
        this->pImpl_Element = new Impl_Element();
    }

    void Element::AppendChildren(Element &element)
    {
        this->pImpl_Element->children.push_back(&element);
    }

    void Element::DefineEventListener(EventListener listener)
    {
        this->pImpl_Element->eventListener = listener;
    }

    void Element::ProcessEventListener(EventImage &eventImage, Element &document)
    {

        if (this->pImpl_Element->active == Logic::True)
        {

            this->pImpl_Element->eventListener(eventImage, *this, document);

            if (this->pImpl_Element->children.size() > 0)
            {
                for (Element *child : this->pImpl_Element->children)
                {
                    child->ProcessEventListener(eventImage, document);
                }
            }
        }
    }

    void Element::DefineLayoutHandler(ElementLayoutHandler layoutHandler)
    {
        this->pImpl_Element->layoutHandler = layoutHandler;
    }

    void Element::ComputeLayout(Element &parent, Element &document)
    {
        if (this->pImpl_Element->active == Logic::True)
        {

            this->pImpl_Element->layoutHandler(parent, *this, document);

            if (this->pImpl_Element->children.size() > 0)
            {
                for (Element *child : this->pImpl_Element->children)
                {
                    child->ComputeLayout(*this, document);
                }
            }
        }
    }

    Vector2D &Element::Position()
    {
        return this->pImpl_Element->panel.Position();
    }

    // void Element::SetPosition(Vector2D position)
    // {
    //     this->pImpl_Element->panel.SetPosition(position);
    // }

    Vector2D &Element::Dimension()
    {
        return this->pImpl_Element->panel.Dimension();
    }

    // void Element::SetDimension(Vector2D dimension)
    // {
    //     return this->pImpl_Element->panel.Resize(dimension);
    // }

    Panel &Element::GetPanel()
    {
        return this->pImpl_Element->panel;
    }

    Logic &Element::Active()
    {
        return this->pImpl_Element->active;
    }

    void Element::ReadyForWindowContext(Window &windowContext)
    {
        this->pImpl_Element->panel.BindWindowContext(windowContext);

        if (this->pImpl_Element->children.size() > 0)
        {
            for (Element *child : this->pImpl_Element->children)
            {
                child->ReadyForWindowContext(windowContext);
            }
        }
    }

    void Element::ClearBuffers()
    {
        this->pImpl_Element->panel.ClearBuffer();

        if (this->pImpl_Element->children.size() > 0)
        {
            for (Element *child : this->pImpl_Element->children)
            {
                child->ClearBuffers();
            }
        }
    }

    void Element::Render()
    {
        if (this->pImpl_Element->active == Logic::True)
        {
            this->pImpl_Element->panel.Retrace();

            if (this->pImpl_Element->children.size() > 0)
            {
                for (Element *child : this->pImpl_Element->children)
                {
                    child->Render();
                }
            }
        }
    }

    std::string &Element::Id()
    {
        return this->pImpl_Element->id;
    }

    Element *Element::FindElementById(std::string id)
    {
        if (this->pImpl_Element->id == id)
            return this;

        if (this->pImpl_Element->children.size() > 0)
        {
            for (Element *child : this->pImpl_Element->children)
            {

                if (Element *found = child->FindElementById(id))
                {
                    return found;
                }
            }
        }

        return nullptr;
    }

    Element::~Element()
    {
        if (this->pImpl_Element)
        {
            delete this->pImpl_Element;
        }
    }

    struct Application::Impl_Application
    {

        Window window;
        Element document;

        Impl_Application(PointDetail windowBufferBaseDetail) : window(windowBufferBaseDetail)
        {

            return;
        }
    };

    Application::Application()
    {
        this->pImpl_Application = new Impl_Application(MakePointDetail(' ', MakeStyle(MakeColor(0, 0, 0), MakeColor(0, 0, 0), OverloadStyle::None)));

        this->pImpl_Application->window.SetCursorVisibleLogic(Logic::False);
        this->pImpl_Application->document.GetPanel().SetPadding(0);

        this->GetDocument().Dimension() = this->GetWindow().GetDimensions();

        this->GetDocument().DefineLayoutHandler([&](Element &parent, Element &This, Element &document)
                                                { This.Dimension() = this->GetWindow().GetDimensions(); });

        this->GetDocument().Id() = "document";

        // this->pImpl_Impl_Application->document.Panel.
    }

    void Application::Render()
    {

        this->GetDocument().ClearBuffers();

        Window &ref_window = this->pImpl_Application->window;

        ref_window.CaptureEventImage();

        EventImage &eventImage = ref_window.GetEventImage();

        if (eventImage.windowEvent.isResizedLogic == Logic::True)
        {
            ref_window.HardClearBuffer();
            this->pImpl_Application->window.Refresh();
        }

        this->pImpl_Application->document.ProcessEventListener(eventImage, this->pImpl_Application->document);

        this->UpdateLayout();

        this->pImpl_Application->document.Render();

        this->pImpl_Application->window.Render();
    }

    void Application::Ready()
    {
        this->pImpl_Application->document.ReadyForWindowContext(this->pImpl_Application->window);
    }

    Window &Application::GetWindow()
    {
        return this->pImpl_Application->window;
    }

    Element &Application::GetDocument()
    {
        return this->pImpl_Application->document;
    }

    void Application::UpdateLayout()
    {

        Window &ref_window = this->pImpl_Application->window;

        Element parent;

        parent.GetPanel().SetPadding(0);

        parent.Dimension() = ref_window.GetDimensions();

        parent.Position() = MakeVector2D(0, 0);

        this->pImpl_Application->document.ComputeLayout(parent, this->pImpl_Application->document);
    }

    int Application::Execute()
    {
        try
        {
            this->Ready();

            while (true)
            {
                this->Render();
            }
            return -1;
        }
        catch (...)
        {
            return -1;
        }
    }

    // Element *Application::FindElementById(std::string id)
    // {
    //     return this->pImpl_Application->document.FindElementById(id);
    // }

    Application::~Application()
    {
        if (this->pImpl_Application)
        {
            delete this->pImpl_Application;
        }
    }
}