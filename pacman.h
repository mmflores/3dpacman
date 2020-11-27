/*
Author: Maya Flores
Class: ECE4122
Last Date Modified: 10/25/20
Description:
dECLARES Classes used in the pacman.cpp file
*/
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h> 

using namespace std;

/*
ghost is made out of sphere head and cylinder body
must have unique color (red, green, orange, pink)
*/
class ECE_Ghost {
    public:
        double radius;
        double bodyHeight;
        float color[3];
        double xyzPos[3];
};

/*
yellow sphere
*/
class ECE_Pacman {
    public:
        double radius;
        float color[4];
        double xyzPos[3];
};

class Maze {
    public:
        double width;
        double radius;
        float color[4];
}; 

