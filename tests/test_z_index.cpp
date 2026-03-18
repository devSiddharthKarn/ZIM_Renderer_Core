#include "../zim/zim.hpp"
#include "../zim/Components/Button.hpp"
using namespace zim;
int main()
{

    Application app;

    // Element element1;
    // element1.GetPanel().Title().sentence="This is a sentence";

    // element1.Dimension() = MakeVector2D(50, 10);
    // element1.Position() = MakeVector2D(0, 0);
    // element1.ZIndex() = 2;
    // element1.GetPanel().SetPadding(0);
    
    // element1.DefineEventListener([&](EventImage& eventImage,Element& This,Element& document){
    //     if(eventImage.eventOccuredLogic==Logic::True){
    //         if(eventImage.mouseEvent.keyPressed==MouseKey::Left){
    //             This.GetPanel().SetBufferBGColor(MakeColor(128, 34, 90));
    //         }else if(eventImage.mouseEvent.keyPressed==MouseKey::Right){
    //             This.GetPanel().SetBufferBGColor(MakeColor(0, 0, 0));
    //         }
    //     }
    //     element1.GetPanel().WriteStr("Hello world from siddharth karn",0,0,Logic::True);
    // });

    Element element;
    element.GetPanel().SetPadding(2);
    element.Dimension()=MakeVector2D(5,5);
    element.Position()=MakeVector2D(10,10);

    Button::Config config;
    config.position=MakeVector2D(2,2);
    config.text=MakeStringDetail("Click Me",MakeStyle(MakeColor(0,0,0),MakeColor(255,255,0),OverloadStyle::Italic));
    config.onClick=[&](EventImage& eventImage,Element& This,Element& document){
        if(eventImage.eventOccuredLogic==Logic::True){
            if(eventImage.mouseEvent.keyPressed==MouseKey::Left){
                element.GetPanel().SetPadding(2);
            }else if(eventImage.mouseEvent.keyPressed==MouseKey::Right){
                // element.GetPanel().SetPadding(1);
                app.SafeQuit();
            }
        }
    };
    Button btn(config);
    btn.element.ZIndex()=UINT32_MAX;
    // element1.AppendChildren(btn.element);

    // Element element2;
    // element2.Dimension() = MakeVector2D(15, 10);
    // element2.Position() = MakeVector2D(0, 0);
    // element2.GetPanel().SetBufferBGColor(MakeColor(128, 34, 90));
    // element2.ZIndex() = 1;

    // app.GetDocument().AppendChildren(element1);
    app.GetDocument().AppendChildren(element);
    app.GetDocument().AppendChildren(btn.element);
    // app.GetDocument().AppendChildren(element2);

    return app.Execute();
}
