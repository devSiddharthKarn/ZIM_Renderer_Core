#include "../zim/zim.hpp"
using namespace zim;
int main()
{

    Application app;

    Element element1;

    element1.Dimension() = MakeVector2D(10, 10);
    element1.Position() = MakeVector2D(0, 0);
    element1.ZIndex() = 2;

    Element element2;
    element2.Dimension() = MakeVector2D(15, 10);
    element2.Position() = MakeVector2D(0, 0);
    element2.GetPanel().SetBufferBGColor(MakeColor(128, 34, 90));
    element2.ZIndex() = 1;

    app.GetDocument().AppendChildren(element1);
    app.GetDocument().AppendChildren(element2);

    return app.Execute();
}
