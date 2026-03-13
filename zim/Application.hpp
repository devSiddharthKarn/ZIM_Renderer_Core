#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#pragma once

#include "Types.hpp"
#include "Window.hpp"
#include "Panel.hpp"

#include "functional"

namespace zim
{

    class Element;

    using ElementLayoutHandler = std::function<void(Element &parent, Element &This,Element& document)>;

    using EventListener = std::function<void(EventImage& eventImage,Element &This,Element& document)>;

    class Element
    {
    private:
        struct Impl_Element;
        struct Impl_Element *pImpl_Element;

    public:
        Element();

        void AppendChildren(Element &element);

        Panel &GetPanel();

        void DefineEventListener(EventListener listener);

        void ProcessEventListener(EventImage& eventImage,Element& document);

        void DefineLayoutHandler(ElementLayoutHandler layoutHandler);

        void ComputeLayout(Element &parent,Element& document);

        Logic &Active();

        Vector2D &Position();

        // void SetPosition(Vector2D position);

        Vector2D &Dimension();

        // void SetDimension(Vector2D dimension);

        void ClearBuffers();

        void Render();

        std::string& Id();

        Element* FindElementById(std::string id);

        Logic RemoveElementById(const std::string& id);

        void ReadyForWindowContext(Window& windowContext);

        ~Element();
    };

    class Application
    {
    private:
        struct Impl_Application;
        struct Impl_Application *pImpl_Application;

        void Ready();
        void UpdateLayout();
        void Render();
    public:
        Application();

        Window &GetWindow();

        Element &GetDocument();

        int Execute();

        ~Application();
    };
}
#endif