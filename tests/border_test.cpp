#include "../zim/zim.hpp"

using namespace zim;

int main(){

    Application app;

    Element el;
    
    // el.DefineLayoutHandler([&](Element& Parent,Element& This,Element& document){
    //     This.Position()=MakeVector2D(-5,10);
    //     This.Dimension()=MakeVector2D(20,5);
    // });

    el.DefineEventListener([&](EventImage& eventImage,Element& This,Element& document){
        if(eventImage.mouseEvent.keyPressed==MouseKey::Left){
            This.GetPanel().TopLeftBorderChar().ch='X';
            This.GetPanel().TopRightBorderChar().ch='X';
            This.GetPanel().BottomLeftBorderChar().ch='X';
            This.GetPanel().BottomRightBorderChar().ch='X';

            This.GetPanel().SetBorderFGColor(MakeColor(255,0,0));
        }else{
            if(eventImage.mouseEvent.keyPressed==MouseKey::Right){
                This.GetPanel().TopLeftBorderChar().ch='+';
                This.GetPanel().TopRightBorderChar().ch='+';
                This.GetPanel().BottomLeftBorderChar().ch='+';
                This.GetPanel().BottomRightBorderChar().ch='+';
                This.GetPanel().SetBorderFGColor(MakeColor(0,255,0));

            }
        }

        This.Dimension()=MakeVector2D(20,5);

        if(eventImage.keyboardEvent.keys.IsEmpty()==Logic::False){

            KeyState keyState=eventImage.keyboardEvent.keys.Get();

            while(keyState.key!=KeyboardKey::None){
                if(keyState.key==KeyboardKey::a){
                    el.Position()=MakeVector2D(el.Position().x-1,el.Position().y);
                }else if(keyState.key==KeyboardKey::d){
                    el.Position()=MakeVector2D(el.Position().x+1,el.Position().y);

                }else if(keyState.key==KeyboardKey::w){
                    el.Position()=MakeVector2D(el.Position().x,el.Position().y-1);

                }else if(keyState.key==KeyboardKey::s){
                    el.Position()=MakeVector2D(el.Position().x,el.Position().y+1);

                }

                keyState=eventImage.keyboardEvent.keys.Get();
            }
        
        }
    });

    app.GetDocument().AppendChildren(el);

    return app.Execute();

}