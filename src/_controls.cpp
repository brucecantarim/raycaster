#include <cmath>
#include <GL/glut.h>
#include "_controls.h"
#include "_maps.h"
#include "_utils.h"

ButtonKeys Keys;

void buttonDown(unsigned char key, int x, int y) {
    if (key == 'a') Keys.a = true;
    if (key == 'd') Keys.d = true;
    if (key == 'w') Keys.w = true;
    if (key == 's') Keys.s = true;
    glutPostRedisplay();
}

void buttonUp(unsigned char key, int x, int y) {
    if (key == 'a') Keys.a = false;
    if (key == 'd') Keys.d = false;
    if (key == 'w') Keys.w = false;
    if (key == 's') Keys.s = false;
    glutPostRedisplay();
}

void buttons(float fps) {
    float increment = 0.2 * fps;
  if (Keys.a) {
    player.a += increment;
    player.a = fixAng(player.a);
    player.dx = cos(degToRad(player.a));
    player.dy = -sin(degToRad(player.a));
  }
  if (Keys.d) {
    player.a -= increment;
    player.a = fixAng(player.a);
    player.dx = cos(degToRad(player.a));
    player.dy = -sin(degToRad(player.a));
  }

    // Colission Logic for Foward and Backward movement
    int xo = 0;
    if (player.dx < 0) { xo = -20; } else {  xo = 20; }
    int yo = 0;
    if (player.dy < 0) { yo = -20; } else {  yo = 20; }
    int ipx = player.x / mapS;
    int ipxAddXo = (player.x + xo) / mapS;
    int ipxSubXo = (player.x - xo) / mapS;
    int ipy = player.y / mapS;
    int ipyAddYo = (player.y + yo) / mapS;
    int ipySubYo = (player.y - yo) / mapS;

  if (Keys.w) {
    if (map[ipy * mapX + ipxAddXo] == 0) player.x += player.dx * increment;
    if (map[ipyAddYo * mapX + ipx] == 0) player.y += player.dy * increment;
  }
  if (Keys.s) {
    if (map[ipy * mapX + ipxSubXo] == 0) player.x -= player.dx * increment;
    if (map[ipySubYo * mapX + ipx] == 0) player.y -= player.dy * increment;
  }
  glutPostRedisplay();
}