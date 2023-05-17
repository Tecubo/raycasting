#include "player.h"

void player_init(Player *p)
{
    p->x = 2.;
    p->y = 2.;
    p->speed = 0.1;
    p->orientation = 0;
    p->rotate_speed = 0.05;
    p->size = 5;
}

void compass_init(Compass *c, int screen_width)
{
    c->x = screen_width - 75;
    c->y = 75;
    c->radius1 = 50;
    c->radius2 = c->radius1 * 1.1;
    c->radius3 = c->radius1 * 1.2;
    c->width = 5;
}

void player_move(Grid map, Player *p, int sens)
{
    double tmpx = p->x + p->speed * cos(p->orientation) * sens;
    double tmpy = p->y + p->speed * sin(p->orientation) * sens;

    if (map.grid[(int)p->y][(int)tmpx] == 0)
        p->x = tmpx;

    if (map.grid[(int)tmpy][(int)p->x] == 0)
        p->y = tmpy;
}

void player_turn(Player *p, int sens)
{
    p->orientation += p->rotate_speed * sens;
}