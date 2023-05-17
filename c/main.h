#ifndef _MAIN_H
#define _MAIN_H 1

#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <err.h>
#include <math.h>
#include "functions.h"
#include "grid.h"
#include "player.h"

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

void SDL_ExitWithError(char *msg);
void change_color(SDL_Renderer *renderer, char *color);
void draw_point(SDL_Renderer *renderer, int x, int y);
void draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int width, char *color);
void fill_rect(SDL_Renderer *renderer, int x, int y, int w, int h, char *color);
void draw_circle(SDL_Renderer *renderer, int x, int y, int radius, int width, char *color);
void fill_circle(SDL_Renderer *renderer, int x, int y, int radius, char *color);
void draw(SDL_Renderer *renderer, Grid map, Player *p, Compass c);
void draw_background(SDL_Renderer *renderer);
void draw_walls(SDL_Renderer *renderer, Player *p, Grid map);
void draw_minimap(SDL_Renderer *renderer, Grid map);
void draw_case(SDL_Renderer *renderer, int x, int y, int size, char *color);
void draw_compass(SDL_Renderer *renderer, Compass c, Player *p);
void draw_player(SDL_Renderer *renderer, Player *p, int case_size);

#endif
