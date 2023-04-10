#ifndef _PLAYER_H
#define _PLAYER_H

typedef struct {
    float x, y, dx, dy, a;
} Player;

extern Player player;

void drawPlayer2D();

#endif
