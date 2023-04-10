#include "_player.h"
#include <GL/glut.h>

Player player;

void drawPlayer2D() {
  glColor3f(1, 1, 0);
  glPointSize(8);
  glLineWidth(4);
  glBegin(GL_POINTS);
  glVertex2i(player.x, player.y);
  glEnd();
  glBegin(GL_LINES);
  glVertex2i(player.x, player.y);
  glVertex2i(player.x + player.dx * 20, player.y + player.dy * 20);
  glEnd();
}
