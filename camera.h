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

typedef struct hud
{
  short counter;
  SDL_Rect box;
} hud;

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
    screen->y = ((player.y + (player.height >> 1)) - (360 >> 1));

    if (screen->x < 0) screen->x = 0;
    else if (screen->x > 640 - screen->w) screen->x = 640 - screen->w;

    if (screen->y < 0) screen->y = 0;
    else if (screen->y > 384 - screen->h) screen->y = 384 - screen->h;
}

void drawHUD(SDL_Renderer *r, SDL_Rect *q)
{
  if (q->w < 20) SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
  else if (q->w < 60) SDL_SetRenderDrawColor(r, 255, 255, 0, 255);
  else SDL_SetRenderDrawColor(r, 0, 255, 0, 255);

  SDL_RenderFillRect(r, q);
}

void updateHUD(hud *h, player *p)
{
  h->box.w--;
  
  if (h->counter % 2 == 0) h->box.x++;

  if (h->counter++ >= 120) 
  {
    h->counter = 0;
    h->box.w = 120;
    h->box.x = 260;
  }

  /*
  if (h->counter > 0) p->run = 1;
  else p->run = 0;
  */
}
