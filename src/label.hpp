#ifndef __LABEL
#define __LABEL

#include "event.hpp"
#include "widget.hpp"

class Label : public EventHandler, public TextContainer, public Position
{
public:
    Label();
    virtual void onRender(int, int) override;
};

#endif