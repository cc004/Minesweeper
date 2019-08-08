#include <GL/glut.h>
#include "button.hpp"

void Button::setSize(int w, int h)
{
    this->w = w;
    this->h = h;
}

int Button::getw()
{
    return w;
}

int Button::geth()
{
    return h;
}

void Button::onClick(int button, int state, int x, int y)
{
    if (x > getx() && y > gety() && x < getx() + w && y < gety() + h)
    {
        if (button == GLUT_LEFT_BUTTON) onLeftClick();
        if (button == GLUT_RIGHT_BUTTON) onRightClick();
    }
}

void Button::onRender(int x, int y)
{
    glColor3i(0, 0, 0);
    glRecti(x + getx(), y + gety(), x + getx() + w, y + gety() + h);
    Label::onRender(x, y);
}

void Button::onLeftClick() {}
void Button::onRightClick() {}