#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "main.h"
#include "_utils.h"
#include "_maps.h"
#include "_textures.h"
#include "_player.h"
#include "_controls.h"

//-----------------------------MAP----------------------------------------------

void drawMap2D() { 
  int x, y, xo, yo;
  for (y = 0; y < mapY; y++) {
    for (x = 0; x < mapX; x++) {
      if (map[y * mapX + x] == 1) {
        glColor3f(1, 1, 1);
      } else {
        glColor3f(0, 0, 0);
      }
      xo = x * mapS;
      yo = y * mapS;
      glBegin(GL_QUADS);
      glVertex2i(0 + xo + 1, 0 + yo + 1);
      glVertex2i(0 + xo + 1, mapS + yo - 1);
      glVertex2i(mapS + xo - 1, mapS + yo - 1);
      glVertex2i(mapS + xo - 1, 0 + yo + 1);
      glEnd();
    }
  }
}

//---------------------------Draw Rays and Walls--------------------------------

void drawCeillingAndFloor2D() {
  glColor3f(0, 1, 1);
  glBegin(GL_QUADS);
  glVertex2i(526, 0);
  glVertex2i(1006, 0);
  glVertex2i(1006, 160);
  glVertex2i(526, 160);
  glEnd();
  glColor3f(0, 0, 1);
  glBegin(GL_QUADS);
  glVertex2i(526, 160);
  glVertex2i(1006, 160);
  glVertex2i(1006, 320);
  glVertex2i(526, 320);
  glEnd();
}

void drawRays2D() {

  int r, mx, my, mp, dof, side;
  float vx, vy, rx, ry, ra, xo, yo, disV, disH;

  ra = fixAng(player.a + 30); // ray set back 30 degrees

  for (r = 0; r < 60; r++) {

    // vertical and horizontal map texture number
    int vmt = 0;
    int hmt = 0;

    //---Vertical---
    dof = 0;
    side = 0;
    disV = 100000;
    float Tan = tan(degToRad(ra));
    if (cos(degToRad(ra)) > 0.001) {
      rx = (((int)player.x >> 6) << 6) + 64;
      ry = (player.x - rx) * Tan + player.y;
      xo = 64;
      yo = -xo * Tan;
    } // looking left
    else if (cos(degToRad(ra)) < -0.001) {
      rx = (((int)player.x >> 6) << 6) - 0.0001;
      ry = (player.x - rx) * Tan + player.y;
      xo = -64;
      yo = -xo * Tan;
    } // looking right
    else {
      rx = player.x;
      ry = player.y;
      dof = 8;
    } // looking up or down. no hit

    while (dof < 8) {
      mx = (int)(rx) >> 6;
      my = (int)(ry) >> 6;
      mp = my * mapX + mx;
      if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
        vmt = map[mp] - 1;
        dof = 8;
        disV = cos(degToRad(ra)) * (rx - player.x) - sin(degToRad(ra)) * (ry - player.y);
      } // hit
      else {
        rx += xo;
        ry += yo;
        dof += 1;
      } // check next horizontal
    }
    vx = rx;
    vy = ry;

    //---Horizontal---
    dof = 0;
    disH = 100000;
    Tan = 1.0 / Tan;
    if (sin(degToRad(ra)) > 0.001) {
      ry = (((int)player.y >> 6) << 6) - 0.0001;
      rx = (player.y - ry) * Tan + player.x;
      yo = -64;
      xo = -yo * Tan;
    } // looking up
    else if (sin(degToRad(ra)) < -0.001) {
      ry = (((int)player.y >> 6) << 6) + 64;
      rx = (player.y - ry) * Tan + player.x;
      yo = 64;
      xo = -yo * Tan;
    } // looking down
    else {
      rx = player.x;
      ry = player.y;
      dof = 8;
    } // looking straight left or right

    while (dof < 8) {
      mx = (int)(rx) >> 6;
      my = (int)(ry) >> 6;
      mp = my * mapX + mx;
      if (mp > 0 && mp < mapX * mapY && map[mp] == 1) {
        hmt = map[mp] - 1;
        dof = 8;
        disH = cos(degToRad(ra)) * (rx - player.x) - sin(degToRad(ra)) * (ry - player.y);
      } // hit
      else {
        rx += xo;
        ry += yo;
        dof += 1;
      } // check next horizontal
    }

    float shade = 1;
    glColor3f(0, 0.8, 0);
    if (disV < disH) {
      hmt = vmt;
      shade = 0.5;
      rx = vx;
      ry = vy;
      disH = disV;
      glColor3f(0, 0.6, 0);
    } // horizontal hit first
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2i(player.x, player.y);
    glVertex2i(rx, ry);
    glEnd(); // draw 2D ray

    int ca = fixAng(player.a - ra);
    disH = disH * cos(degToRad(ca)); // fix fisheye
    int lineH = (mapS * 320) / (disH);

    float tyStep = 32.0 / (float)lineH;
    float tyOff = 0;
    
    if (lineH > 320) {
      tyOff = (lineH - 320) / 2.0;
      lineH = 320;
    }                                 // line height and limit
    int lineOff = 160 - (lineH >> 1); // line offset

    int y;
    float ty = tyOff * tyStep + hmt * 32;
    float tx;
    if (shade == 1) {
      tx = (int) (rx / 2.0) % 32;
      if (ra > 180) tx = 31 - tx;
    } else {
      tx = (int) (ry / 2.0) % 32;
      if (ra > 90 && ra < 270) tx = 31 - tx;
    }
    
    for (y = 0; y < lineH; y++) {
      float c = All_Textures[(int) (ty) * 32 + (int) (tx)] * shade;
      glColor3f(c, c, c);
      glPointSize(8);
      glBegin(GL_POINTS);
      glVertex2i(r * 8 + 530, y + lineOff);
      glEnd(); // draw vertical wall
      ty += tyStep;
    }

    ra = fixAng(ra - 1); // go to next ray
  }
}

void init() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("Raycaster");
  glClearColor(0.3, 0.3, 0.3, 0);
  gluOrtho2D(0, WIDTH, HEIGHT, 0);
  player.x = 150;
  player.y = 400;
  player.a = 90;
  player.dx = cos(degToRad(player.a));
  player.dy = -sin(degToRad(player.a));
}

float frame1, frame2, fps;

void display() {
  frame2 = glutGet(GLUT_ELAPSED_TIME);
  fps = (frame2 - frame1);
  frame1 = glutGet(GLUT_ELAPSED_TIME);
  buttons(fps); // TODO: move to timer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawMap2D();
  drawPlayer2D();
  drawCeillingAndFloor2D();
  drawRays2D();
  glutSwapBuffers();
}

void update() {
  glutDisplayFunc(display);
  glutKeyboardFunc(buttonDown);
  glutKeyboardUpFunc(buttonUp);
  glutMainLoop();
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  init();
  update();
}
