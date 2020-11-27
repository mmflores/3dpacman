/*
Author: Maya Flores
Class: ECE4122
Last Date Modified: 10/29/20
Description:
Creates OpenGL window and renders a 3d image of a packman board.
*/

#include <iostream>
//#include <GL/glew.h> // Include the GLEW header file
#include <GL/glut.h> // Include the GLUT header file
#include <stdio.h>
#include <math.h>
#include <stdlib.h> // standard definitions
#include "pacman.h"
#include <thread>

using namespace std;

#define ESC 27

GLUquadricObj *quadratic = gluNewQuadric();

ECE_Pacman Pacman;
ECE_Ghost Ghosts[4];
Maze maze;

//declares ghost flat colors
float ghostColorsFlat[4][3] = {
    {1.0, 0.0, 0.0},       //red
    {1.0, 0.5, 0.0},       //orange
    {0.251, 0.878, 0.816}, //green
    {0.91, 0.62, 0.77}     //pink

};

//declares coordinates for vertical maze walls startpoints and endpoints
float vMazeWalls[30][4] = {
    //outside walls
    {0, 0, 0, 7},
    {0, 14, 0, 20},
    {17, 0, 17, 7},
    {17, 14, 1, 20},
    {3, 7, 3, 9},
    {3, 12, 3, 14},
    {14, 7, 14, 9},
    {14, 12, 14, 14},

    //center box
    {6, 9, 6, 12},
    {11, 9, 11, 12},

    //bottom half vertical lines
    {4.5, 1.5, 4.5, 3.5},
    {12.5, 1.5, 12.5, 3.5},
    {8.5, 1.5, 8.5, 3.5},
    {2.5, 3.5, 2.5, 5.5},
    {14.5, 3.5, 14.5, 5.5},

    //center vertical lines
    {8.5, 5.5, 8.5, 7.5},
    {4.5, 7.5, 4.5, 9.5},
    {12.5, 7.5, 12.5, 9.5},
    {4.5, 12.5, 4.5, 15.5},
    {12.5, 12.5, 12.5, 15.5},
    {8.5, 13.5, 8.5, 15.5},

    //top vertical lines
    {8.5, 17, 8.5, 20},
    {1.5, 17.5, 1.5, 18.5},
    {2.5, 17.5, 2.5, 18.5},
    {4.5, 17.5, 4.5, 18.5},
    {6.5, 17.5, 6.5, 18.5},
    {10.5, 17.5, 10.5, 18.5},
    {12.5, 17.5, 12.5, 18.5},
    {14.5, 17.5, 14.5, 18.5},
    {15.5, 17.5, 15.5, 18.5}};

//declares coordinates for horizontal maze walls startpoints and endpoints
float hMazeWalls[35][4] = {
    //outside walls
    {0, 0, 17, 0},
    {0, 20, 17, 20},
    {0, 7, 3, 7},

    {0, 9, 3, 9},
    {0, 12, 3, 12},

    {0, 14, 3, 14},
    {14, 7, 17, 7},
    {14, 9, 17, 9},
    {14, 12, 17, 12},
    {14, 14, 17, 14},

    //center box
    {6, 9, 11, 9},
    {6, 12, 11, 12},

    //bottom half horizontal lines
    {1.5, 1.5, 6.5, 1.5},
    {10.5, 1.5, 14.5, 1.5},
    {0, 3.5, 1, 3.5},
    {16, 3.5, 17, 3.5},
    {6.5, 3.5, 10.5, 3.5},
    {1.5, 5.5, 2.5, 5.5},
    {14.5, 5.5, 15.5, 5.5},
    {4.5, 5.5, 6.5, 5.5},
    {10.5, 5.5, 12.5, 5.5},

    //middle horizontal lines
    {6, 7.5, 11, 7.5},
    {4.5, 13.5, 6.5, 13.5},
    {10.5, 13.5, 12.5, 13.5},
    {6, 15.5, 11, 15.5},
    {1.5, 15.5, 2.5, 15.5},
    {14.5, 15.5, 15.5, 15.5},

    //top horizontal lines
    {1.5, 17.5, 2.5, 17.5},
    {1.5, 18.5, 2.5, 18.5},
    {4.5, 17.5, 6.5, 17.5},
    {4.5, 18.5, 6.5, 18.5},
    {10.5, 17.5, 12.5, 17.5},
    {10.5, 18.5, 12.5, 18.5},
    {14.5, 17.5, 15.5, 17.5},
    {14.5, 18.5, 15.5, 18.5}};

//martrix that represents coins, 0 means empty, 1 means coin, 2 means disk
int coinMatrix[20][17] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
    {0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0},
    {2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {2, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 2},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}

    /*
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {2,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,2},
    {1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,1,0,0,0,0,0,1,0,1,0,0,1},
    {1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0},
    {1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1},
    {2,1,0,1,1,1,1,1,1,1,1,1,1,1,0,1,2},
    {0,1,0,1,0,1,0,0,0,0,0,1,0,1,0,1,0},
    {1,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,1},
    {1,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    */
};

int wallMatrix[22][19] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
    {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
    {0, 2, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
    {0, 2, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 2, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

// Camera position
float x = 0.0, y = -5.0, z = 1.0; // initially 5 units south of origin
float deltaMove = 0.0;            // initially camera doesn't move

// Camera direction
float lx = 16.0, ly = 1.0; // camera points initially along y-axis
float angle = 0.0;         // angle of rotation for the camera direction
float deltaAngle = 0.0;    // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

//colors used for rendering light
float colorDarkWhite[4] = {0.1f, 0.1f, 0.1f, 1.0f};
float colorOffWhite[4] = {1.0f, 248.0f / 255.0f, 220.0f / 255.0f, 1.0f};
float colorWhite[4] = {.60, 0.60, 0.60, 1.0f};
float colorBlack[4] = {0.01, 0.01, 0.01, 1.0f};
float colorBlue[4] = {0.0, 0.0, 1.0, 1.0f};
float colorDarkBlue[4] = {0.098, 0.098, 0.439, 1.0f};
float colorYellow[4] = {0.941, 0.902, 0.549, 1.0f};
float colorDarkYellow[4] = {1.000, 1.000, 0.000, 1.0f};
float colorGold[4] = {0.741, 0.718, 0.420, 1.0f};
float colorDarkGold[4] = {0.855, 0.647, 0.125, 1.0f};
float colorSilver[4] = {0.753, 0.753, 0.753, 1.0f};
float colorDarkGray[4] = {0.412, 0.412, 0.412, 1.0f};
float ghostColors[4][4] = {
    {0.251, 0.878, 0.816, 1.0f},
    {1.000, 0.000, 0.000, 1.0f},
    {1.000, 0.647, 0.000, 1.0f},
    {1.000, 0.714, 0.757, 1.0f}};
float ghostColorsDark[4][4] = {
    {0.255, 0.412, 0.882, 1.0f},
    {0.545, 0.000, 0.000, 1.0f},
    {0.824, 0.412, 0.118, 1.0f},
    {0.780, 0.082, 0.522, 1.0f}};
GLfloat shininess[] = {5};

bool bIsLightingOn = false;
int sceneRotationAngle = 0;
float dis = 80.0f;
GLfloat position[] = {80.0f, 80.0f, 80.0f, 1.0f};

//----------------------------------------------------------------------
// Reshape callback
//
// Window size has been set/changed to w by h pixels. Set the camera
// perspective to 45 degree vertical field of view, a window aspect
// ratio of w/h, a near clipping plane at depth 1, and a far clipping
// plane at depth 100. The viewport is the entire window.
//
//----------------------------------------------------------------------
void changeSize(int w, int h)
{
    float ratio = ((float)w) / ((float)h);      // window aspect ratio
    glMatrixMode(GL_PROJECTION);                // projection matrix is active
    glLoadIdentity();                           // reset the projection
    gluPerspective(20.0, ratio, 0.1, 100000.0); // perspective transformation
    glMatrixMode(GL_MODELVIEW);                 // return to modelview mode
    glViewport(0, 0, w, h);                     // set viewport (drawing area) to entire window
}

//function that draws maze walls
void drawMaze()
{
    if (bIsLightingOn)
    {
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, colorDarkBlue);
        glMaterialfv(GL_FRONT, GL_AMBIENT, colorDarkBlue);
        glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlue);
    }
    else
    {
        glColor3f(0, 0, 1.0); // set drawing color to based on i
    }

    //draw vertical walls
    for (int i = 0; i < 30; i++)
    {
        glPushMatrix();
        //x, y, z coordinates of start
        glTranslatef(2 * vMazeWalls[i][0], 2 * vMazeWalls[i][1], 0.1);
        //rotate around x axis
        glRotatef(-90.0, 1.0, 0.0, 0.0);
        //draw cylinder
        gluCylinder(quadratic, 0.25, 0.25, 2 * (vMazeWalls[i][3] - vMazeWalls[i][1]), 100, 100);
        glPopMatrix();
    }

    //draw horizonatal walls
    for (int i = 0; i < 35; i++)
    {
        glPushMatrix();
        //x, y, z coordinates of start
        glTranslatef(2 * hMazeWalls[i][0], 2 * hMazeWalls[i][1], 0.1);
        //rotate around x axis
        glRotatef(90.0, 0.0, 1.0, 0.0);
        //draw cylinder
        gluCylinder(quadratic, 0.25, 0.25, 2 * (hMazeWalls[i][2] - hMazeWalls[i][0]), 100, 100);
        glPopMatrix();
    }
}

//function that renders coins and disks
void drawCoins()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 17; j++)
        {
            //draw orange disk
            if (coinMatrix[i][j] == 2)
            {
                //set drawing color
                if (bIsLightingOn)
                {
                    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorGold);
                    glMaterialfv(GL_FRONT, GL_AMBIENT, colorDarkGold);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, colorDarkGold);
                }
                else
                {
                    glColor3f(0.855, 0.647, 0.125);
                }

                glPushMatrix();
                //x, y, z coordinates of start
                glTranslatef((2 * j + 1), 2 * i + 1, 0.1);

                //draw disk
                //gluCylinder(quadratic,0.5, 0.5, .2, 100, 100);
                gluDisk(quadratic, 0, 0.4, 100, 100);
                glPopMatrix();
            }
            //draw coin
            else if (coinMatrix[i][j] == 1)
            {
                //set drawing color
                if (bIsLightingOn)
                {
                    glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
                    glMaterialfv(GL_FRONT, GL_DIFFUSE, colorDarkGray);
                    glMaterialfv(GL_FRONT, GL_AMBIENT, colorDarkGray);
                    glMaterialfv(GL_FRONT, GL_SPECULAR, colorSilver);
                }
                else
                {
                    glColor3f(0.753, 0.753, 0.753);
                }

                glPushMatrix();
                //x, y, z coordinates of start
                glTranslatef(2 * j + 0.5, 2 * i + 0.5, 0.1);

                //draw cylinder
                glutSolidSphere(0.2, 100, 100);
                glPopMatrix();
            }
        }
    }
}

void movePacman(int dir)
{
    int x = Pacman.xyzPos[0];
    int y = Pacman.xyzPos[1];

    //get dir
    switch (dir)
    {
    case 0: //right
        x = x + 1;
        break;
    case 1: //left
        x = x - 1;
        break;
    case 2: //up
        y = y+1;
        break;
    case 3: //down
        y = y-1;
        break;
    }

    //check if x,y is a wall
    //if not a wall move pacman
    if (wallMatrix[x][y] != 0){
        Pacman.xyzPos[0] = x; //set x pos
        Pacman.xyzPos[1] = y; //set y pos
        drawPacman();
    }
}

//function that renders pacman
void drawPacman()
{
    if (bIsLightingOn)
    {
        glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, colorYellow);
        glMaterialfv(GL_FRONT, GL_AMBIENT, colorDarkYellow);
        glMaterialfv(GL_FRONT, GL_SPECULAR, colorDarkYellow);
    }
    else
    {
        glColor3f(Pacman.color[0], Pacman.color[1], Pacman.color[2]); // set drawing color to based on i
    }

    glPushMatrix();
    glTranslatef(Pacman.xyzPos[0], Pacman.xyzPos[1], 0.75);
    glutSolidSphere(0.95, 100, 100);
    glPopMatrix();
}

//function that renders all four ghosts
void drawGhosts()
{
    for (int i = 0; i < 4; i++)
    {
        if (bIsLightingOn)
        {
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, ghostColors[i]);
            glMaterialfv(GL_FRONT, GL_AMBIENT, ghostColors[i]);
            glMaterialfv(GL_FRONT, GL_SPECULAR, ghostColorsDark[i]);
        }
        else
        {
            glColor3f(Ghosts[i].color[0], Ghosts[i].color[1], Ghosts[i].color[2]); // set drawing color to based on i
        }

        glPushMatrix();
        glTranslatef(2 * (i + 7), 20.5, .75);
        gluCylinder(quadratic, 0.8, 0.8, 1, 100, 100);
        glTranslatef(0, 0, .8);
        glutSolidSphere(0.8, 100, 100);
        glPopMatrix();
    }
}

//----------------------------------------------------------------------
// Update with each idle event
//
// This incrementally moves the camera and requests that the scene be
// redrawn.
//----------------------------------------------------------------------
void update(void)
{
    if (deltaMove)
    { // update camera position
        x += deltaMove * lx * 0.1;
        y += deltaMove * ly * 0.1;
    }
    glutPostRedisplay(); // redisplay everything
}

void renderScene(void)
{
    int i, j;

    // Clear color and depth buffers
    glClearColor(0.0, 0.0, 0.0, 1.0); // black background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();

    // Set the camera centered at (x,y,1) and looking along directional
    // vector (lx, ly, 0), with the z-axis pointing up
    gluLookAt(
        dis, -dis, dis, // camera location
        0.0, 0.0, 0.0,  // where camera is pointed
        0.0, 0.0, 1.0); // landscape or portrait

    // Rotate the wholescene by an angle around the z-axis
    glRotatef(sceneRotationAngle, 0.0f, 0.0f, 1.0f);

    // Move the light with the scene
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // Draw ground - 200x200 square colored green
    if (bIsLightingOn)
    {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, colorBlack);
        glMaterialfv(GL_FRONT, GL_AMBIENT, colorBlack);
        glMaterialfv(GL_FRONT, GL_SPECULAR, colorBlack);
    }
    else
    {
        glColor3f(0.0, 0.0, 0.0);
    }

    drawMaze();

    drawCoins();

    drawPacman();

    drawGhosts();

    glutSwapBuffers(); // Make it all visible
}

void toggleLighting()
{
    bIsLightingOn = !bIsLightingOn;

    if (bIsLightingOn)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, colorOffWhite);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, colorOffWhite);
        glLightfv(GL_LIGHT0, GL_SPECULAR, colorDarkWhite);
    }
    else
    {
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
    }
    glutPostRedisplay();
}
//----------------------------------------------------------------------
// User-input callbacks
//
// processNormalKeys: ESC, q, and Q cause program to exit
// pressSpecialKey: Up arrow = forward motion, down arrow = backwards
// releaseSpecialKey: Set incremental motion to zero
//----------------------------------------------------------------------
void processNormalKeys(unsigned char key, int xx, int yy)
{
    if (key == ESC || key == 'q' || key == 'Q')
    {
        exit(0);
    }
    else if (key == 'd' || key == 'D')
    {
        dis -= 1.0;
        ;
    }
    else if (key == 'u' || key == 'U')
    {
        dis += 2.0;
        ;
    }
    else if (key == 'r' || key == 'R')
    {
        sceneRotationAngle = (sceneRotationAngle + 5) % 360;
    }
    else if (key == 'l' || key == 'L')
    {
        toggleLighting();
    }
}

void SpecialKeys(int key, int x, int y)
{
    int dir = 0;
    switch (key)
    {
    case GLUT_KEY_LEFT:
        dir = 1;
        break;
    case GLUT_KEY_RIGHT:
        dir = 0;
        break;
    case GLUT_KEY_UP:
        dir = 2;
        break;
    case GLUT_KEY_DOWN:
        dir = 3;
        break;
    }

    movePacman(dir);
}

//----------------------------------------------------------------------
// Process mouse drag events
//
// This is called when dragging motion occurs. The variable
// angle stores the camera angle at the instance when dragging
// started, and deltaAngle is a additional angle based on the
// mouse movement since dragging started.
//----------------------------------------------------------------------
void mouseMove(int x, int y)
{
    if (isDragging)
    { // only when dragging
        // update the change in angle
        deltaAngle = (x - xDragStart) * 0.005;

        // camera's direction is set to angle + deltaAngle
        lx = -sin(angle + deltaAngle);
        ly = cos(angle + deltaAngle);
    }
}

void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {                   // left mouse button pressed
            isDragging = 1; // start dragging
            xDragStart = x; // save x where button first pressed
        }
        else
        {                        /* (state = GLUT_UP) */
            angle += deltaAngle; // update camera turning angle
            isDragging = 0;      // no longer dragging
        }
    }
}
void init()
{
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
}

void glutMainLoop(void)
{
}

//----------------------------------------------------------------------
// Main program  - standard GLUT initializations and callbacks
//----------------------------------------------------------------------
int main(int argc, char **argv)
{
    printf("\n\
-----------------------------------------------------------------------\n\
  OpenGL Sample Program:\n\
  - Drag mouse left-right to rotate camera\n\
  - Hold up-arrow/down-arrow to move camera forward/backward\n\
  - q or ESC to quit\n\
-----------------------------------------------------------------------\n");

    // general initializations
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 400);
    glutCreateWindow("Pacman");
    init();
    // register callbacks
    glutReshapeFunc(changeSize);          // window reshape callback
    glutDisplayFunc(renderScene);         // (re)display callback
    glutIdleFunc(update);                 // incremental update
                                          //    glutIgnoreKeyRepeat(1); // ignore key repeat when holding key down
    glutMouseFunc(mouseButton);           // process mouse button push/release
    glutMotionFunc(mouseMove);            // process mouse dragging motion
    glutKeyboardFunc(processNormalKeys);  // process standard key clicks
    glutSpecialFunc(SpecialKeys);         //move pacman keys
    toggleLighting();
    // Initial Pacman
    Pacman.radius = 0.75;
    Pacman.color[0] = 1.0;
    Pacman.color[1] = 1.0;
    Pacman.color[2] = 0.0;
    Pacman.xyzPos[0] = 17;
    Pacman.xyzPos[1] = 9;

    // Initial Ghosts
    for (int i = 0; i < 4; i++)
    {
        Ghosts[i].color[0] = ghostColorsFlat[i][0];
        Ghosts[i].color[1] = ghostColorsFlat[i][1];
        Ghosts[i].color[2] = ghostColorsFlat[i][2];
    }

    //init maze
    maze.color[0] = 0.0;
    maze.color[1] = 0.2;
    maze.color[2] = 0.7;

    if (!quadratic)
        quadratic = gluNewQuadric();

    // enter GLUT event processing cycle
    glutMainLoop();

    return 0; // this is just to keep the compiler happy
}