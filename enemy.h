#include "player.h"

typedef struct enemy
{
    short   x, y, 
            w, h, 
            vel:2,
            velFrames:4,
            respawnFrames:8;

    bool dead:1;

    enum facing f:1;
} enemy;

void setEnemies(enemy e[])
{
    for (int i = 0; i < 5; i++)
    {
        e[i].w = 18;
        e[i].h = 14;

        switch (i)
        {
            case 0:
                e[i].x = 8 << 5;
                e[i].y = (10 << 5) - 14;
                break;
            case 1:
                e[i].x = 6 << 5;
                e[i].y = (8 << 5) - 14;
                break;
            case 2:
                e[i].x = 11 << 5;
                e[i].y = (8 << 5) - 14;
                break;
            case 3:
                e[i].x = 2 << 5;
                e[i].y = (10 << 5) - 14;
                break;
            case 4:
                e[i].x = 17 << 5;
                e[i].y = (10 << 5) - 14;
                break;
        }

        e[i].vel = 1;
        e[i].velFrames = 0;

        e[i].respawnFrames = 0;

        e[i].dead = false;

        e[i].f = RIGHT;
    }
}

void update_enemy(enemy *e, player *p)
{
    if (e->dead)
    {
        e->respawnFrames++;

        if (e->respawnFrames == 120) 
        {
            e->respawnFrames = 0;
            e->dead = false;
        }
        return;
    }

    short   left = e->x >> 5, 
            right = e->x + e->w >> 5,
            top = e->y >> 5,
            bottom = e->y + e->h >> 5;

    //  test collision logic here
    if (p->state == FALLING && (p->y + p->height < e->y))
    {
        if (((p->x + p->width >= e->x) && (p->x <= e->x + e->w))
        && (p->y + p->height + p->yVel >= e->y)) 
        {
            e->dead = true;
            if (p->bounceFrames == 0)   //  make player bounce !!!
            {
                p->bounce = true;
                p->y = e->y - p->height;

                p->yVel = 2;    // needs tweaking ?
            }
            return;
        }
    }

    if (e->f == RIGHT) e->vel = 1;
    else if (e->f == LEFT) e->vel = -1;

    if (e->vel)
    {
        if (!(1 << left & map[bottom])) 
        {
            e->x = (left + 1 << 5);
            e->f = RIGHT;
        }
        else if (!(1 << right & map[bottom])) 
        {
            e->x = (right << 5) - e->w;
            e->f = LEFT;
        }
    }

    e->velFrames++;

    if (!(e->velFrames % 4)) e->x += e->vel;
}

void update_enemies(enemy enemies[], player *p)
{
    for (int i = 0; i < 5; i++)
    {
        update_enemy(&enemies[i], p);
    }
}
