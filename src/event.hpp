#ifndef __EVENT
#define __EVENT

#include <string>
#include <vector>

class EventHandler
{
public:
    virtual void onClick(int, int, int, int);
    virtual void onTick(int);
    virtual void onRender(int, int);
};

class EventBus : public EventHandler
{
    std::vector<EventHandler *> handlers;
public:
    void regEvent(EventHandler&);
    virtual void onClick(int, int, int, int) override;
    virtual void onTick(int) override;
    virtual void onRender(int, int) override;
};

#endif