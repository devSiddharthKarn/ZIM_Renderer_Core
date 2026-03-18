#ifndef ZIM_COMPONENT_BUTTON_HPP
#define ZIM_COMPONENT_BUTTON_HPP
#include "Basic_includes.hpp"
#include "../zim.hpp"
#include "ciso646"

class Button
{
public:
    struct Config
    {
        zim::StringDetail text;
        zim::EventListener onClick;
        zim::Vector2D position;
    };

private:
public:
    zim::Element element;
    Button(Config config)
    {
        

        element.DefineLayoutHandler([this,config](zim::Element &Parent, zim::Element &This, zim::Element &document)
                                    {
            This.GetPanel().SetPadding(0);
            This.Dimension()=zim::MakeVector2D(config.text.sentence.length()+This.GetPanel().GetPadding()*2,1);
            This.Position()=config.position; 
        });

        element.DefineEventListener([this,config](zim::EventImage &eventImage, zim::Element &This, zim::Element &document)
                                    {
                                        zim::Vector2D mousePos = eventImage.mouseEvent.position;
                                        zim::Vector2D elementPos = This.Position();
                                        zim::Vector2D elementDim = This.Dimension();

                                        zim::Color bgColor;
                                        zim::Color fgColor;

                                        if (mousePos.x >= elementPos.x + This.GetPanel().GetPadding() && mousePos.x < elementPos.x + elementDim.x - This.GetPanel().GetPadding() && mousePos.y >= elementPos.y + This.GetPanel().GetPadding() && mousePos.y < elementPos.y + elementDim.y - This.GetPanel().GetPadding())
                                        {
                                            if (eventImage.eventOccuredLogic == zim::Logic::True && eventImage.mouseEvent.keyPressed == zim::MouseKey::Left)
                                            {
                                                config.onClick(eventImage, This, document);
                                            }

                                            bgColor = config.text.styles.bg_color;
                                            fgColor = config.text.styles.fg_color;

                                            This.GetPanel().SetBufferBGColor(bgColor);
                                            This.GetPanel().SetBufferFGColor(fgColor);
                                            This.GetPanel().WriteStrDetailed(config.text.sentence, zim::MakeVector2D(0, 0), zim::Logic::False, MakeStyle(bgColor, fgColor, config.text.styles.overloadStyle));
                                        }
                                        else
                                        {

                                            bgColor = config.text.styles.fg_color;
                                            fgColor = config.text.styles.bg_color;

                                            This.GetPanel().SetBufferBGColor(bgColor);
                                            This.GetPanel().SetBufferFGColor(fgColor);
                                            This.GetPanel().WriteStrDetailed(config.text.sentence, zim::MakeVector2D(0, 0), zim::Logic::False, MakeStyle(bgColor, fgColor, config.text.styles.overloadStyle));
                                        }
                                    });
    }
};

#endif