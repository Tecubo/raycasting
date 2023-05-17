#ifndef _PLAYER_H
#define _PLAYER_H 1

#include "grid.h"
#include <math.h>

typedef struct Player Player;
struct Player
{
    double x;
    double y;
    double speed;
    double orientation;
    double rotate_speed;
    int size;
};

typedef struct Compass Compass;
struct Compass
{
    int x;
    int y;
    int radius1;
    int radius2;
    int radius3;
    int width;
};

void player_init(Player *p);
void compass_init(Compass *c, int screen_width);
void player_move(Grid map, Player *p, int sens);
void player_turn(Player *p, int sens);

#endif