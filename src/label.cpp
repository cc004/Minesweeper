#include <GL/gl.h>
#include "label.hpp"

Label::Label() : TextContainer("")
{

}

void Label::onRender(int x, int y)
{
    renderText(x + getx(), y + gety());
}