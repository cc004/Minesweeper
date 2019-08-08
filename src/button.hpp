#ifndef __BUTTON
#define __BUTTON

#include "label.hpp"

class Button : public Label
{
    int w, h;
public:
    void setSize(int, int);
    int getw();
    int geth();
    virtual void onClick(int, int, int, int) override;
    virtual void onRender(int, int) override;
    virtual void onLeftClick();
    virtual void onRightClick();
};

#endif