#include "main.h"

int main()
{
    /* Initialisation des variables */

    Player *p = malloc(sizeof(Player));
    errMaloc(p, __FILE__, __LINE__);
    player_init(p);

    Compass c = {SCREEN_WIDTH - 75, 75, 50, 50 * 1.1, 50 * 1.2};

    Grid map = define_map();

    /* Initialisation de la SDL et de la fenêtre */

    if (SDL_Init(SDL_INIT_VIDEO))
        SDL_ExitWithError("Erreur lors de l'initialisation de la SDL");

    SDL_Window *window = SDL_CreateWindow("RayCasting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (window == NULL)
        SDL_ExitWithError("Erreur lors de la création de la fenêtre");

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
        SDL_ExitWithError("La création du rendu a échouée");

    /* Boucle principale du jeu */

    SDL_bool running = SDL_TRUE;

    while (running)
    {
        SDL_Event event;
        draw(renderer, map, p, c);

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_z:
                    player_move(map, p, 1);
                    continue;
                case SDLK_s:
                    player_move(map, p, -1);
                    continue;
                case SDLK_q:
                    player_turn(p, 1);
                    continue;
                case SDLK_d:
                    player_turn(p, -1);
                    continue;
                }
                continue;

            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            }
        }

        SDL_RenderPresent(renderer);
    }

    /* Destruction des variables en mémoires */

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    destroy_map(map);
    free(p);

    return EXIT_SUCCESS;
}

void SDL_ExitWithError(char *msg)
{
    printf("Error: %s\n", msg);
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void change_color(SDL_Renderer *renderer, char *color)
{
    if (color == "RED")
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    else if (color == "BLACK")
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    else if (color == "WHITE")
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    else if (color == "GREY")
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, SDL_ALPHA_OPAQUE);
    else if (color == "SKY")
        SDL_SetRenderDrawColor(renderer, 134, 177, 255, SDL_ALPHA_OPAQUE);
    else
    {
        printf("Warning: the color you choosed does not exist, black is set by default");
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    }
}

void draw_point(SDL_Renderer *renderer, int x, int y)
{
    if (SDL_RenderDrawPoint(renderer, x, SCREEN_HEIGHT - y))
        SDL_ExitWithError("Impossible de tracer le point");
}

void draw_line(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int width, char *color)
{
    change_color(renderer, color);
    x1 -= (int)(width / 2);
    x2 -= (int)(width / 2);
    y1 -= (int)(width / 2);
    y2 -= (int)(width / 2);
    for (int i = 0; i < width; i++)
    {
        if (SDL_RenderDrawLine(renderer, x1 + i, SCREEN_HEIGHT - (y1 + i), x2 + i, SCREEN_HEIGHT - (y2 + i)))
            SDL_ExitWithError("Impossible de tracer la ligne");
    }
}

void fill_rect(SDL_Renderer *renderer, int x, int y, int w, int h, char *color)
{
    change_color(renderer, color);
    SDL_Rect rect = {x, SCREEN_HEIGHT - y - h, w, h};
    if (SDL_RenderFillRect(renderer, &rect))
        SDL_ExitWithError("Impossible de tracer le rectangle");
}

void draw_circle(SDL_Renderer *renderer, int x, int y, int radius, int width, char *color)
{
    change_color(renderer, color);
    double angle = 0;
    for (double i = 0; i < 1000; i++)
    {
        angle = i / 1000 * 2 * M_PI;
        for (int w = 0; w < width; w++)
            draw_point(renderer, x + cos(angle) * (radius - w), y + sin(angle) * (radius - w));
    }
}

void fill_circle(SDL_Renderer *renderer, int x, int y, int radius, char *color)
{
    draw_circle(renderer, x, y, radius, radius, color);
}

void draw(SDL_Renderer *renderer, Grid map, Player *p, Compass c)
{
    draw_background(renderer);
    draw_walls(renderer, p, map);
    draw_minimap(renderer, map);
    draw_compass(renderer, c, p);
    draw_player(renderer, p, map.size);
}

void draw_background(SDL_Renderer *renderer)
{
    fill_rect(renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 2, "WHITE");
    fill_rect(renderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2, "SKY");
}

void draw_walls(SDL_Renderer *renderer, Player *p, Grid map)
{
    double ratio;
    double dirX, dirY;
    int mapX, mapY;
    double deltadistX, deltadistY;
    double sideDistX, sideDistY;
    int stepX, stepY;
    int hit, side;
    double perpwall;

    for (double i = 0; i < SCREEN_WIDTH; i++)
    {
        hit = 0;
        side = 0;
        perpwall = 0;
        ratio = (i - SCREEN_WIDTH / 2) / (SCREEN_WIDTH / 2);
        dirX = cos(p->orientation) / 2 + cos(p->orientation - M_PI / 2) * ratio;
        dirY = sin(p->orientation) / 2 + sin(p->orientation - M_PI / 2) * ratio;
        mapX = (int)p->x;
        mapY = (int)p->y;
        deltadistX = sqrt(1 + pow(dirY / dirX, 2));
        deltadistY = sqrt(1 + pow(dirX / dirY, 2));
        if (dirX > 0)
            sideDistX = deltadistX * (1 - (p->x - mapX));
        else
            sideDistX = deltadistX * (p->x - mapX);
        if (dirY > 0)
            sideDistY = deltadistY * (1 - (p->y - mapY));
        else
            sideDistY = deltadistY * (p->y - mapY);
        stepX = dirX < 0 ? -1 : 1;
        stepY = dirY < 0 ? -1 : 1;

        while (!hit)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltadistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltadistY;
                mapY += stepY;
                side = 1;
            }
            hit = map.grid[mapY][mapX];
        }

        if (side)
            perpwall = (mapY - p->y + (1 - stepY) / 2) / dirY;
        else
            perpwall = (mapX - p->x + (1 - stepX) / 2) / dirX;

        draw_line(renderer, i, SCREEN_HEIGHT / 2 - (SCREEN_HEIGHT / 2) / perpwall, i, SCREEN_HEIGHT / 2 + (SCREEN_HEIGHT / 2) / perpwall, 1, side ? "BLACK" : "GREY");
    }
}

void draw_minimap(SDL_Renderer *renderer, Grid map)
{
    for (int y = 0; y < map.height; y++)
    {
        for (int x = 0; x < map.width; x++)
        {
            if (map.grid[y][x])
                draw_case(renderer, x, y, map.size, "GREY");
            else
                draw_case(renderer, x, y, map.size, "WHITE");
        }
    }
}

void draw_case(SDL_Renderer *renderer, int x, int y, int size, char *color)
{
    fill_rect(renderer, x * size, y * size, size, size, color);
}

void draw_compass(SDL_Renderer *renderer, Compass c, Player *p)
{
    fill_circle(renderer, c.x, c.y, c.radius3, "BLACK");
    fill_circle(renderer, c.x, c.y, c.radius2, "WHITE");
    draw_line(renderer, c.x, c.y, c.x + cos(p->orientation) * c.radius1, c.y + sin(p->orientation) * c.radius1, 5, "RED");
}

void draw_player(SDL_Renderer *renderer, Player *p, int case_size)
{
    fill_rect(renderer, p->x * case_size, p->y * case_size, p->size, p->size, "RED");
}
