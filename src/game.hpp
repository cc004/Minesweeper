#ifndef __GAME
#define __GAME

#include "event.hpp"
#include "button.hpp"

class Game;
class GameMap;

class RemainLabel : public Label
{
    GameMap& map;
public:
    RemainLabel(GameMap&);
    virtual void onRender(int, int) override;
};

enum Status
{
    blank_empty,
    mark_empty,
    unknown_empty,
    open_empty,
    blank_mine,
    mark_mine,
    unknown_mine,
    open_mine,
    known_mine
};

enum GameResult
{
    playing,
    dead,
    completed,
    uninitialized
};

class GameMap : public Position, public EventHandler
{
    Status** map;
    int w, h, n;
    bool mineplaced;
    int getNear(int, int) const;
    void renderElement(int, int, int, int, GameResult) const;
    void spread(int, int);
public:
    static const int wsize, hsize;
    GameMap();
    static std::vector<int> randmine(int, int);
    void placeMine(int, int);
    void initMap(int, int, int);
    int getw() const;
    int geth() const;
    int getn() const;
    GameResult getStatus() const;
    int getRemain() const;
    virtual void onClick(int, int, int, int) override;
    virtual void onRender(int, int) override;
    ~GameMap();
    void clearMap();
};

class Game : public EventHandler
{
    GameMap map;
    EventBus bus;
    Label time;
    RemainLabel remain;
    int startTick, nowTick;
public:
    static const int border, msg, height;
    Game();
    void setSize(int, int, int);
    void restart();
    virtual void onClick(int, int, int, int) override;
    virtual void onTick(int) override;
    virtual void onRender(int, int) override;
};

#endif