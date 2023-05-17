#ifndef _GRID_H
#define _GRID_H 1

#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

typedef struct Grid Grid;
struct Grid
{
    int width;
    int height;
    int size;
    int **grid;
};

Grid define_map();
void print_map(Grid map);
void destroy_map(Grid map);

#endif