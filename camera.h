#include "enemy.h"

int lerp(int v0, int v1, float t) 
{
  return (1 - t) * v0 + t * v1;
}

typedef struct camera
{
    unsigned char adjustCounter:4, xVel:4, yVel:4;

    SDL_Rect box;
} camera;

void setCamera(SDL_Rect *screen, player player)
{
  /*
    const int   cpX = ((player.x + (player.width >> 1)) - (640 >> 1)), 
                lpX = ((player.x + (player.width >> 1)) - (640 >> 1)) - TILE_SIZE, 
                rpX = ((player.x + (player.width >> 1)) - (640 >> 1)) + TILE_SIZE;

    int pX;

    if (player.xVel < 0) pX = lpX;
    else if (player.xVel > 0) pX = rpX;
    else pX = cpX;
  */
    screen->x = ((player.x + (player.width >> 1)) - (640 >> 1));
    screen->y = ((player.y + (player.height > 1)) - (360 >> 1));

    if (screen->x < 0) screen->x = 0;
    else if (screen->x > 640 - screen->w) screen->x = 640 - screen->w;

    if (screen->y < 0) screen->y = 0;
    else if (screen->y > 768 - screen->h) screen->y = 768 - screen->h;
}
