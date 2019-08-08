#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>
#include <GL/GLUT.h>
#include "game.hpp"

Game game;
std::thread *thr;
bool changesize;

struct
{
    int w, h, n;
} input;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1,1,0);
    game.onRender(0, 0);
	glFlush();
}

void mouse(int button, int state, int x, int y)
{
    game.onClick(button, state, x, y);
    glutPostRedisplay();
}

void timer(int value)
{
    game.onTick(value);
    if (changesize)
    {
        game.setSize(input.w, input.h, input.n);
        changesize = false;
    }
    glutTimerFunc(100, timer, value + 1);
    glutPostRedisplay();
}

void reshape(int w, int h)
{
   //scale to make point(0, 0) locate in the left-top corner 
    glViewport(0, 0, w, h);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);
}

void cinHandler()
{
    int w, h, n;
    while(true)
    {

        std::cin >> w >> h >> n;
        if (!std::cin)
        {
            std::cin.clear();
            std::cin.sync();
        }
        else
        {
            input.w = w;
            input.h = h;
            input.n = n;
            changesize = true;
        }
    }
}

void init()
{
    srand(time(NULL));
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowSize(320, 240);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Minesweeper");

	glClearColor(0, 0, 1, 0);
    //Add hooks to gameloop
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(100, timer, 0);
    glutReshapeFunc(reshape);

    changesize = false;
    //Handle keyboard input in another thread so that it won't stuck the main thread
    thr = new std::thread(cinHandler);
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    init();
    glutMainLoop();
    return 0;
}
