#include <GL/GL.h>
#include "widget.hpp"

void Position::setPosition(int x, int y)
{
    this->x = x;
    this->y = y;
}

int Position::getx() const
{
    return x;
}

int Position::gety() const
{
    return y;
}

bool TextContainer::init = false;
int TextContainer::textmap = 0;

TextContainer::TextContainer(std::string str) : text(str)
{
    offset.setPosition(0, 0);
}

void TextContainer::setOffset(int x, int y)
{
    offset.setPosition(x, y);
}

Position TextContainer::getOffset() const
{
    return offset;    
}

void TextContainer::setText(std::string str)
{
    text = str;
}

void TextContainer::setColor(int Color)
{
    this->Color = Color;
}

std::string TextContainer::getText() const
{
    return text;
}

int TextContainer::getColor() const
{
    return Color;
}

void TextContainer::renderText(int x, int y) const
{
    //Initialize the font cache
    if (!init)
    {
        textmap = glGenLists(256);
        wglUseFontBitmaps(wglGetCurrentDC(), 0, 256, textmap);
        init = true;
    }
    const char *str = text.c_str();
    glRasterPos2i(x + offset.getx(), y + offset.gety());
    //render it to screen, do not support Chinese characters
    while (*str) glCallList(textmap + (*str++));
}