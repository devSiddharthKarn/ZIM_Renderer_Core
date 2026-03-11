#include "zim.hpp"
using namespace zim;
int main()
{

    Application app;

    Element element;

    // element.DefineLayoutHandler([](Element &parent, Element &This)
    //                             {
    //     This.SetPosition(MakeVector2D(2,2));
    //     This.SetDimension(MakeVector2D(10,10)); });

    element.SetDimension(MakeVector2D(30,20));

    element.DefineEventListener([](EventImage &eventImage, Element &This)
                                          {
                                              if (eventImage.eventOccuredLogic == Logic::False)
                                                  return;

                                              Vector2D m_pos = eventImage.mouseEvent.position;

                                              Vector2D pos = This.GetPosition();

                                              Vector2D dimen = This.GetDimension();

                                              if ((m_pos.x >= pos.x && m_pos.y >= pos.y) && (m_pos.x <= pos.x + dimen.x && m_pos.y <= pos.y + dimen.y))
                                              {

                                                  This.GetPanel().SetBorderFGColor(MakeColor(255, 255, 0));

                                                  std::string str = std::string("") + "(" + std::to_string(m_pos.x) + "," + std::to_string(m_pos.y) + ")";

                                                  This.GetPanel().ClearBuffer(MakePointDetail(' ', MakeStyle(MakeColor(0, 0, 0), MakeColor(255, 255, 255))));
                                                  This.GetPanel().WriteStr(str, 0, 0, Logic::False);

                                                  if (eventImage.mouseEvent.keyPressed != MouseKey::None)
                                                  {
                                                    This.SetDimension(MakeVector2D(20,10));
                                                  }else{
                                                    This.SetDimension(MakeVector2D(30,20));
                                                  }
                                              }
                                              else
                                              {
                                                  This.GetPanel().SetBorderFGColor(MakeColor(128, 128, 128));
                                              }
                                          });

    Element children;

    children.DefineLayoutHandler([](Element &parent, Element &This)
                                 {
        This.SetPosition(MakeVector2D(3,3));
        This.SetDimension(MakeVector2D(5,5)); });

    children.DefineEventListener([](EventImage &eventImage, Element &This)
                                 {
            if(eventImage.eventOccuredLogic==Logic::False) return;
            
            Vector2D m_pos = eventImage.mouseEvent.position;
            
            Vector2D pos = This.GetPosition();
            
            Vector2D dimen = This.GetDimension();
            
            if((m_pos.x>=pos.x && m_pos.y>=pos.y) && (m_pos.x<=pos.x+dimen.x && m_pos.y<=pos.y+dimen.y)){
                
                This.GetPanel().SetBorderFGColor(MakeColor(255,255,0));
                
                This.GetPanel().WriteStr("Hello world",0,0,Logic::False);
                
            }else{
                This.GetPanel().SetBorderFGColor(MakeColor(128,128,128));
            } });

    // element.AppendChildren(children);
    app.GetDocument().AppendChildren(element);

    return app.Execute();
}