#include "../zim/zim.hpp"
using namespace zim;
int main(){

    Application app;

    Element element;
    
    element.DefineLayoutHandler([](Element& Parent,Element& This,Element& document){
        This.Position()=MakeVector2D(10,10);
        This.Dimension()=MakeVector2D(10,10);
    });

    element.Id()="element";

    app.GetDocument().AppendChildren(element);

    app.GetDocument().DefineEventListener([&](EventImage& eventImage,Element& This,Element& document){
        if(eventImage.eventOccuredLogic==Logic::True){
            if(eventImage.mouseEvent.keyPressed==MouseKey::Left){
                document.RemoveElementById("element");
            }
        }
    });


    return app.Execute();


}