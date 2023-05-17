#include "grid.h"

Grid define_map()
{
    FILE *file = fopen("map.txt", "r");
    if (file == NULL)
        errx(1, "Error: failed to open the map file");

    Grid map = {0, 0, 10, NULL};

    while (fgetc(file) == '1')
        (map.width)++;

    rewind(file);

    char *line = malloc(sizeof(char) * (map.width + 2));
    while (fgets(line, map.width + 2, file))
        (map.height)++;
    free(line);

    rewind(file);

    char c = '0';
    map.grid = malloc(sizeof(int *) * map.height);
    errMaloc(map.grid, __FILE__, __LINE__);

    for (int y = 0; y < map.height; y++)
    {
        map.grid[y] = malloc(sizeof(int) * map.width);
        errMaloc(map.grid[y], __FILE__, __LINE__);

        for (int x = 0; x < map.width; x++)
        {
            c = fgetc(file);
            if (c == '1')
                map.grid[y][x] = 1;
            else if (c == '0')
                map.grid[y][x] = 0;
            else
                errx(1, "Error: unknown caracter '%c' map.txt:%d:%d in the map file", c, x, y);
        }
        fgetc(file);
    }

    fclose(file);

    return map;
}

void print_map(Grid map)
{
    for (int y = 0; y < map.height; y++)
    {
        for (int x = 0; x < map.width; x++)
        {
            printf("%d ", map.grid[y][x]);
        }
        printf("\n");
    }
}

void destroy_map(Grid map)
{
    for (int y = 0; y < map.height; y++)
        free(map.grid[y]);
    free(map.grid);
}