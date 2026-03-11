#include "../zim/Application.hpp"

int main(){
    using namespace zim;


    Application app;

    Element ui;

    ui.Position()=MakeVector2D(10,10);
    ui.Dimension()=MakeVector2D(12,12);

    app.GetDocument().AppendChildren(ui);

    return app.Execute();
}