#include "../zim/Application.hpp"
#include "string"
int main(){
    using namespace zim;


    Application app;

    Element ui;

    ui.Position()=MakeVector2D(10,10);
    ui.Dimension()=MakeVector2D(12,12);

    ui.DefineEventListener([&](EventImage& eventImage,Element& This,Element& document){
        if(eventImage.eventOccuredLogic==Logic::True){
            if(eventImage.mouseEvent.keyPressed==MouseKey::Left){
                app.SafeQuit();
            }else if(eventImage.mouseEvent.keyPressed==MouseKey::Right){
                std::string buffer = std::string("(")+std::to_string(eventImage.mouseEvent.position.x)+","+std::to_string(eventImage.mouseEvent.position.y)+")";

                ui.GetPanel().Title().sentence=buffer;
            }

            std::string buffer = std::string("(")+std::to_string(eventImage.mouseEvent.position.x)+","+std::to_string(eventImage.mouseEvent.position.y)+")";

                ui.GetPanel().Title().sentence=buffer;

        }
    });

    app.GetDocument().AppendChildren(ui);

    return app.Execute();
}