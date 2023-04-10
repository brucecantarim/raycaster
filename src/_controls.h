#ifndef _CONTROLS_H
#define _CONTROLS_H

#include "_player.h"

typedef struct {
    bool w, a, s, d; // button states on / off
} ButtonKeys;

void buttonDown(unsigned char key, int x, int y);
void buttonUp(unsigned char key, int x, int y);
void buttons(float fps);

extern Player player;

#endif