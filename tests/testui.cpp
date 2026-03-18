#include "../zim/Application.hpp"

int main(){
    using namespace zim;


    Application app;

    Element ui;

    ui.Position()=MakeVector2D(10,10);
    ui.Dimension()=MakeVector2D(12,12);

    ui.DefineEventListener([&](EventImage& eventImage,Element& This,Element& document){
        if(eventImage.eventOccuredLogic==Logic::True){
            if(eventImage.mouseEvent.keyPressed==MouseKey::Right){
                app.SafeQuit();
            }
        }
    });

    app.GetDocument().AppendChildren(ui);

    return app.Execute();
}