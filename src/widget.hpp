#ifndef __WIDGET
#define __WIDGET

#include <string>

class Position
{
protected:
    int x, y;
public:
    void setPosition(int, int);
    int getx() const;
    int gety() const;
};

class TextContainer
{
    static bool init;
    static int textmap;
    std::string text;
    Position offset;
    int Color;
public:
    TextContainer(std::string);
    void setOffset(int, int);
    Position getOffset() const;
    void setText(std::string);
    void setColor(int);
    std::string getText() const;
    int getColor() const;
    void renderText(int, int) const;
};

#endif