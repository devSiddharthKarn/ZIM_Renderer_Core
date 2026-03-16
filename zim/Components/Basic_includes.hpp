#ifndef ZIM_COMPONENTS_BASIC_INCLUDES_HPP
#define ZIM_COMPONENTS_BASIC_INCLUDES_HPP

#include "functional"
#include "../zim.hpp"

struct OptionClickAction{
    zim::StringDetail option;
    zim::EventListener eventListener;
};

#endif