#ifndef ZIM_BUILTIN_COMPONENTS_MESSAGE_BOX
#define ZIM_BUILTIN_COMPONENTS_MESSAGE_BOX

#include "Basic_includes.hpp"
#include "../zim.hpp"
#include "vector"

class MessageBox{
    public:
    struct Config{
        uint32_t ZIndex=UINT32_MAX;
        zim::StringDetail title;
        zim::StringDetail message;
        std::vector<OptionClickAction> optionsAndClickAction;

        static Config Generate(){
            
        }
    };
    private:
    Config config;
    public:
    zim::Element element;
    std::vector<std::string> lines;
    std::vector<zim::Element> options;


    void MakeOptions(){
        this->options.clear();

        for(OptionClickAction& option:this->config.optionsAndClickAction){
            
        }
    }

    MessageBox(Config config){
        this->config = config;

        element.GetPanel().Title() = this->config.title;

        

        element.DefineLayoutHandler([&](zim::Element& parent,zim::Element& This,zim::Element& document){
            This.Dimension()=zim::MakeVector2D(parent.Dimension().x*0.3,parent.Dimension().y*0.3);
            float x_pos =  parent.Dimension().x*0.5-This.Dimension().x*0.5;
            float y_pos = parent.Dimension().y*0.5-This.Dimension().y*0.5;
            This.Position()=zim::MakeVector2D(x_pos,y_pos);
        });
    }


};


#endif