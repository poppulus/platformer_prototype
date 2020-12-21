#include <stdbool.h>
#include <SDL2/SDL.h>
#include "map.h"

const int JOYSTICK_DEAD_ZONE = 8000;

enum player_state
{
    STANDING,
    WALKING,
    JUMPING,
    FALLING
};

enum player_gamepad
{
    A = 1,
    B = 2,
    X = 4,
    Y = 8
};

enum facing {RIGHT, LEFT};

typedef struct player 
{
    short   x, y, 
            width, height, 
            xVel, yVel,
            velocity;

    bool    bounce:1, 
            jumped:1, 
            landed:1, 
            running:1,
            hasJumped:1,
            run:1;

    unsigned char yVelFactor:4, xVelFactor:4, bounceFrames:3, moving:2;

    enum facing facing:1;
    enum player_state state;
} player;

void handlePadEvents(SDL_Event *e, player *p)
{
    if (e->type == SDL_JOYBUTTONDOWN && e->jbutton.which == 0)
    {
        switch (e->jbutton.button)
        {
            case 0:
                if (!p->hasJumped && (p->state == STANDING || p->bounce)) 
                {
                    if (p->bounce)
                    {
                        p->yVel = -6;
                        p->bounceFrames = 0;
                        p->bounce = false;
                    }
                    else 
                    {
                        if (p->run) p->yVel = -5;
                        else p->yVel = -4;
                    }
                    
                    p->jumped = true;
                    p->hasJumped = true;
                }
            break;
            case 2:
                p->run = true;
            break;
            
        }
    }

    if (e->type == SDL_JOYBUTTONUP && e->jbutton.which == 0)
    {
        switch (e->jbutton.button)
        {
            case 0:
                p->hasJumped = false;
            break;
            case 2:
                p->run = false;
            break;
        }
    }

    if (e->type == SDL_JOYAXISMOTION && e->jaxis.which == 0)
    {
        if (e->jaxis.axis == 0 && e->jaxis.value > JOYSTICK_DEAD_ZONE)
        {
            p->moving = 2;
            p->facing = RIGHT;
        }
        else if (e->jaxis.axis == 0 && e->jaxis.value < -JOYSTICK_DEAD_ZONE)
        {
            p->moving = 1;
            p->facing = LEFT;
        }
        else if (e->jaxis.axis == 0 
        && (e->jaxis.value < JOYSTICK_DEAD_ZONE 
        && e->jaxis.value > -JOYSTICK_DEAD_ZONE)) 
        {
            p->moving = 0;
        }
        
    }
}

void handleKeyEvents(SDL_Event *e, player *p)
{
    if (e->type == SDL_KEYDOWN && e->key.repeat == 0)
    {
        switch (e->key.keysym.sym)
        {
            case SDLK_LSHIFT:
                p->run = 1;
                break;
            case SDLK_LEFT:
                p->moving = 1;
                p->facing = LEFT;
                break;
            case SDLK_RIGHT:
                p->moving = 2;
                p->facing = RIGHT;
                break;
            case SDLK_SPACE:
                if (!p->hasJumped && (p->state == STANDING || p->bounce)) 
                {
                    if (p->bounce)
                    {
                        if (p->run) p->yVel = -6;
                        else p->yVel = -5;
                        
                        p->bounceFrames = 0;
                        p->bounce = false;
                    }
                    else 
                    {
                        if (p->run) p->yVel = -5;
                        else p->yVel = -4;
                    }
                    
                    p->jumped = true;
                    p->hasJumped = true;
                }
                break;
        }
    }

    if (e->type == SDL_KEYUP)
    {
        switch (e->key.keysym.sym)
        {
            case SDLK_LSHIFT:
                p->run = 0;
                break;
            case SDLK_LEFT:
                if (p->moving == 1) p->moving = 0;
                break;
            case SDLK_RIGHT:
                if (p->moving == 2) p->moving = 0;
                break;
            case SDLK_SPACE:
                p->hasJumped = false;
                break;
        }
    }
}

bool checkXVel(player *p)
{
    const short left = (p->x + p->xVel) >> 5,
                right = (p->x + p->xVel + p->width) >> 5,
                top = p->y >> 5,
                bottom = (p->y + p->height) >> 5;

    switch (p->state)
    {
        case FALLING:
            if (p->xVel > 0)
            {
                if (((1 << right) & map[top]) 
                || ((1 << right) & map[bottom])) return true;
            }
            else if (p->xVel < 0)
            {
                if (((1 << left) & map[top]) 
                || ((1 << left) & map[bottom])) return true;
            }
            break;
        case JUMPING:
            if (p->xVel < 0) 
            {
                if (p->jumped)
                {
                    if ((1 << left) & map[top]) 
                    {
                        p->x = (left + 1 << 5) + 1;
                        return true;
                    }
                }
                else if (((1 << left) & map[top]) 
                || ((1 << left) & map[bottom])) return true; 
            }
            else if (p->xVel > 0)
            {
                if (p->jumped)
                {
                    if ((1 << right) & map[top]) 
                    {
                        p->x = (right << 5) - p->width - 1;
                        return true;
                    }
                }
                else if (((1 << right) & map[top]) 
                || ((1 << right) & map[bottom])) return true;
            }
            break;
    }

    return false;
}

void render_player(player *player, SDL_Rect clips[])
{
    
}

void setPlayer(player *player)
{
    player->width = 18;
    player->height = 24;

    player->x = 320;
    player->y = 300;

    player->xVel = 0;
    player->yVel = 0;

    player->velocity = 0;
    player->moving = 0;

    player->xVelFactor = 0;
    player->yVelFactor = 0;
    
    player->bounceFrames = 0;

    player->state = FALLING;
    player->facing = RIGHT;

    player->bounce = false;
    player->jumped = false;
    player->landed = false;
    player->running = false;
    player->hasJumped = false;
    player->run = false;
}

void update_player(player *player)
{
    const short leftX = (player->x + player->xVel) >> 5, 
                rightX = (player->x + player->xVel + player->width) >> 5, 
                topY = (player->y + player->yVel) >> 5,
                bottomY = (player->y + player->yVel + player->height) >> 5;

    if (player->x < 0) player->x = 0;
    else if (player->x + player->width > 640) player->x = 640 - player->width;

    if (player->y < 0) 
    {
        player->y = 0;
        player->yVel = 0;
    }
    else if (player->y + player->height > 384) player->y = 384 - player->height;

    if (player->yVel > 0) player->state = FALLING;
    else if (player->yVel < 0) player->state = JUMPING;

    if (player->moving == 1) 
    {
        if (player->run) player->velocity = -3;
        else player->velocity = -2;
    }
    else if (player->moving == 2) 
    {
        if (player->run) player->velocity = 3;
        else player->velocity = 2;
    }
    else player->velocity = 0;

    switch (player->velocity)
    {
        case 3:
            player->running = true;
        case 2:
            if (player->xVel++ >= player->velocity) player->xVel = player->velocity;
            break;
        case -3:
            player->running = true;
        case -2:
            if (player->xVel-- <= player->velocity) player->xVel = player->velocity;
            break;
        case 0: 
            player->running = false;
        default: 
            if (player->xVel < player->velocity) player->xVel++;
            else if (player->xVel > player->velocity) player->xVel--;
            break;
    }

    switch (player->state)
    {
        case STANDING:
            if (player->xVel)
            {
                if (player->xVel < 0) 
                {
                    if (1 << leftX & map[topY]) player->x = (leftX + 1 << 5) + 1;
                }
                else if (player->xVel > 0)
                {
                    if (1 << rightX & map[topY]) player->x = (rightX << 5) - player->width - 2;
                }

                if (!(1 << leftX & map[bottomY]) && !(1 << rightX & map[bottomY]))
                {
                    player->state = FALLING;
                }
            }
            break;
        case FALLING:
            player->landed = false;

            if (checkXVel(player)) player->xVel = 0;
            
            if (player->bounce)
            {
                if (player->bounceFrames++ == 2) 
                {
                    player->yVel = -2;
                    player->xVel = 0;

                    player->jumped = true;

                    player->state = JUMPING;
                    break;
                }
            }

            player->yVelFactor += 1;

            if (player->yVelFactor % 4 == 0) player->yVel += 1;

            if (((1 << (player->x >> 5)) & map[bottomY] 
            && !(1 << (player->x + player->width >> 5) & map[topY])) 
            || ((1 << (player->x + player->width >> 5)) & map[bottomY] 
            && !(1 << (player->x + player->width >> 5) & map[topY])))
            {
                player->y = (bottomY << 5) - player->height;

                player->yVelFactor = 0;
                player->yVel = 0;

                player->state = STANDING;
                player->landed = true;
            }
            break;
        case JUMPING:
            player->landed = false;
            
            if (checkXVel(player)) player->xVel = 0;

            player->jumped = false;

            if (player->bounce)
            {
                player->yVelFactor += 1;

                if (player->yVelFactor % 2 == 0) player->yVel += 1;

                if (player->bounceFrames++ == 5) 
                {
                    player->bounceFrames = 0;
                    player->bounce = false;
                }
                break;
            }

            if (player->hasJumped)
            {
                player->yVelFactor += 1;

                if (player->yVelFactor % 4 == 0) player->yVel += 1;

                if (((1 << (player->x >> 5)) & map[topY] 
                && !(1 << (player->x >> 5) & map[bottomY])) 
                || ((1 << (player->x + player->width >> 5)) & map[topY] 
                && !(1 << (player->x + player->width >> 5) & map[bottomY])))
                {   
                    player->yVel = 0;
                    player->state = FALLING;
                }
            }
            else
            {
                player->yVel = 0;
                player->state = FALLING;
            }
            break;
        default:
            break;
    }

    player->x += player->xVel;
    player->y += player->yVel;
}
