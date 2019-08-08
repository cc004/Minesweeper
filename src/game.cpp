#include <cstring>
#include <GL/glut.h>
#include "game.hpp"
#include <cstdlib>

//represent the eight directions
static const std::vector<int> dir = {-1, 0, -1, -1, -1, 1, 0, 1, 0, -1, 1, 0, 1, 1, 1, -1};

RemainLabel::RemainLabel(GameMap& map) : map(map) {}

void RemainLabel::onRender(int x, int y)
{
    int remain;
    std::string str = "Remain: 000";
    switch(map.getStatus())
    {
        case uninitialized: //if the user haven't clicked anywhere.
            remain = map.getn();
            str[8] = remain / 100 + '0'; //split into 3-digit numbers
            str[9] = remain / 10 % 10 + '0';
            str[10] = remain % 10 + '0';
            setText(str);
            break;
        case playing:
            remain = map.getRemain();
            str[8] = remain / 100 + '0';
            str[9] = remain / 10 % 10 + '0';
            str[10] = remain % 10 + '0';
            setText(str);
            break;
        case dead:
            setText("Game Over");
            break;
        case completed:
            setText("You win!");
            break;
    }
    renderText(x + getx(), y + gety());
}

const int GameMap::wsize = 20, GameMap::hsize = 20; //size of single square in pixel

int GameMap::getNear(int x, int y) const //returns how many mines are near (x, y)
{
    int s = 0;
    for (int i = 0; i < 8; ++i)
    {
        int xt = x + dir[2 * i], yt = y + dir[2 * i + 1];
        if (xt >= 0 && yt >= 0 && xt < w && yt < h && map[xt][yt] & 4)
            ++s;
    }
    return s;
}

void GameMap::renderElement(int i, int j, int x, int y, GameResult status) const //render a single square
{
    TextContainer text("");
    text.setOffset(6, 14); //to ensure the text is drawn at the center when we give the position of the left-top corner.
    char str[2] = {'0', 0};
    switch(map[i][j])
    {
        case unknown_empty:
        case unknown_mine:
            text.setText("?");
            break;
        case blank_mine:
            if (status == playing) break; //do not mark out the mine when the player haven't end the game.
        case mark_mine:
        case mark_empty:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(1.0, 0.5, 0.0);
            glRecti(x, y, x + wsize, y + hsize);
            break;
        case open_mine:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(1.0, 0.0, 0.0);
            glRecti(x, y, x + wsize, y + hsize);
            break;
        case open_empty:
            str[0] += getNear(i, j);
            if (str[0] == '0') //mark empty when there's no mines nearby.
            {                
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                glColor3f(0.5, 0.5, 0.5);
                glRecti(x, y, x + wsize, y + hsize);
            }
            else text.setText(std::string(str));
            break;
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(0.0, 0.0, 0.0);
    glRecti(x, y, x + wsize, y + hsize);
    text.renderText(x, y);
}

void GameMap::spread(int x, int y) //open square(x,y)
{
    if (map[x][y] & 3) return;
    map[x][y] = (Status)((map[x][y] & 4) | 3);
    if (!getNear(x, y))
    {
        for (int i = 0; i < 8; ++i)
        {
            int xt = x + dir[2 * i], yt = y + dir[2 * i + 1];
            if (xt >= 0 && yt >= 0 && xt < w && yt < h && !(map[xt][yt] & 3))
                spread(xt, yt);
        }
    }
}

GameMap::GameMap() : map(NULL) {}

std::vector<int> GameMap::randmine(int m, int n) //select m numbers in 1~n randomly
{
    std::vector<int> res, res2;
    for (int i = 0; i < n; ++i) res.insert(res.begin() + rand() % (i + 1), i);
    for (int i = 0; i < m; ++i)
    {
        res2.push_back(res[i]);
    }
    return res2;
}

void GameMap::placeMine(int x, int y) //place mine to map when the player click (x, y)
{
    int avoid = y * w + x;
    for (int mine : randmine(n, w * h - 1))
    {
        if (mine >= avoid) ++mine;
        map[mine % w][mine / w] = blank_mine;
    }
}

void GameMap::initMap(int w, int h, int n) //generate a pure blank map with parameters
{
    if (n > w * h - 1) return;
    clearMap();
    map = new Status*[w];
    for (int i = 0; i < w; ++i)
    {
        map[i] = new Status[h];
        memset(map[i], 0, sizeof(Status) * h);
    }
    this->w = w;
    this->h = h;
    this->n = n;
    mineplaced = false;
}

int GameMap::getw() const
{
    return map ? w : 0;
}

int GameMap::geth() const
{
    return map ? h : 0;
}

int GameMap::getn() const
{
    return map ? n : 0;
}

//TODO: calculate status only when the gamemap changed to improve performance.
GameResult GameMap::getStatus() const //calculate the status of the map, if the game finished and so on.
{
    if (!mineplaced) return uninitialized;
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j)
            if (map[i][j] == open_mine) return dead;
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j)
            if (!(map[i][j] & 4) && map[i][j] != open_empty) return playing;
    return completed;
}

int GameMap::getRemain() const //calculate the number of remaining mines in the map.
{
    int s = 0;
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j)
        {
            if (map[i][j] & 4) ++s;
            if ((map[i][j] & 3) == mark_empty) --s;
        }
    return s;
}

void GameMap::onClick(int button, int state, int x, int y)
{
    int i = (x - getx()) / wsize, j = (y - gety()) / hsize;
    if (i < 0 || j < 0 || i >= w || j >= h || state) return;

    if (!mineplaced)
    {
        placeMine(i, j);
        mineplaced = true;
    }

    if (button == GLUT_LEFT_BUTTON)
        spread(i, j);

    if (button == GLUT_RIGHT_BUTTON && (map[i][j] & 3) != 3)
    {
        map[i][j] = (Status)((map[i][j] & 4) | (((map[i][j] & 3) + 1) % 3)); //switch 3 different status.
    }
}

void GameMap::onRender(int x, int y)
{
    x += getx();
    y += gety();
    if (!map) return;
    GameResult status = getStatus();
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j)
            renderElement(i, j, x + i * wsize, y + j * wsize, status);
}

GameMap::~GameMap()
{
    clearMap();
}

void GameMap::clearMap()
{
    if (map) //release memory allocation
    {
        for (int i = 0; i < w; ++i) delete[] map[i];
        delete[] map;
    }
}

const int Game::border = 20, Game::msg = 120, Game::height = 40;

Game::Game() : remain(this->map)
{
    bus.regEvent(map);
    bus.regEvent(time);
    bus.regEvent(remain);
    map.setPosition(msg + border, border);
    remain.setPosition(border, border + height);
    time.setPosition(border, border + 3 * height);
    time.setText("Time:");
}

void Game::setSize(int w, int h, int n)
{
    map.initMap(w, h, n);
    glutReshapeWindow(w * GameMap::wsize + msg + border * 2, 2 * border + GameMap::hsize * h);
}

void Game::restart()
{
    map.initMap(map.getw(), map.geth(), map.getn());
    onRender(0, 0);
}

void Game::onClick(int button, int state, int x, int y)
{
    int status = map.getStatus();
    if (status == playing || status == uninitialized) bus.onClick(button, state, x, y);
}

void Game::onTick(int tick)
{
    GameResult status = map.getStatus();
    if (status == playing) nowTick = tick;
    if (status == uninitialized)
    {
        startTick = tick;
        nowTick = tick;
    }
    int dt = nowTick - startTick;
    std::string str = "Time: mm:ss.i";
    str[6] = dt < 6000 ? ' ' : (dt / 6000) + '0';
    str[7] = (dt / 600 % 10) + '0';
    str[9] = dt % 600 / 100 + '0';
    str[10] = dt / 10 % 10 + '0';
    str[12] = dt % 10 + '0';
    time.setText(str);
    bus.onTick(tick);
}

void Game::onRender(int x, int y)
{
    bus.onRender(x, y);
}