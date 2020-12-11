#include "camera.h"
#include "texture.h"

const int FPS = 60;
const int TICKS = 1000 / FPS;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 360;

const int TOTAL_TILES =  240;

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}

bool setTiles(tile set[]) 
{
    bool success = true;

    FILE *map = fopen("test.map", "r");

    int c = 0, x = 0, y = 0;

    if (map == NULL) success = false;
    else 
    {
        for (int i = 0; i < TOTAL_TILES; i++)
        {
            fscanf(map, "%d", &c);

            set[i].type = c;
            set[i].box.w = 32;
            set[i].box.h = 32;
            set[i].box.x = x;
            set[i].box.y = y;

            switch (c)
            {
                case 1:
                    set[i].set[0] = tile_set[0];
                    set[i].set[1] = tile_set[1];
                    set[i].set[2] = tile_set[2];
                    set[i].set[3] = tile_set[3];
                    break;
                case 2:
                    set[i].set[0] = tile_set_2[0];
                    set[i].set[1] = tile_set_2[1];
                    set[i].set[2] = tile_set_2[2];
                    set[i].set[3] = tile_set_2[3];
                    break;
                case 0: default: break;
            }

            x += 32;

            if (x >= 640) 
            {
                x = 0; 
                y += 32;
            }
        }
    }

    fclose(map);

    return success;
}

void render_tiles(      // this function checks super tiles,
    texture *sheet,     // and renders every tile in that set
    tile tileSet[],  
    SDL_Rect *tileClips, 
    SDL_Rect camera)  
{
    //  test tile rendering here!!! //
    for (int i = 0; i < TOTAL_TILES; i++)
    {
        if (checkCollision(tileSet[i].box, camera) && tileSet[i].type > 0) 
        {
            renderTexture(sheet, tileSet[i].box.x - camera.x, tileSet[i].box.y - camera.y, &tileClips[tileSet[i].set[0]], SDL_FLIP_NONE);
            renderTexture(sheet, tileSet[i].box.x + 16 - camera.x, tileSet[i].box.y - camera.y, &tileClips[tileSet[i].set[1]], SDL_FLIP_NONE);
            renderTexture(sheet, tileSet[i].box.x - camera.x, tileSet[i].box.y + 16 - camera.y, &tileClips[tileSet[i].set[2]], SDL_FLIP_NONE);
            renderTexture(sheet, tileSet[i].box.x + 16 - camera.x, tileSet[i].box.y + 16 - camera.y, &tileClips[tileSet[i].set[3]], SDL_FLIP_NONE);
        }
    }
    //  test tile rendering here!!! //
}

void gameLoop(
    SDL_Joystick *controller,
    texture *character, 
    texture *sheet, 
    SDL_Rect charClips[], 
    SDL_Rect tileClips[])
{
    bool quit = false;
    bool INPUT_JOYSTICK = false;

    int frame = 0, timer = 0;

    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

    SDL_Event e;
    SDL_Rect *currentClip;
    SDL_Rect *enemyClip;

    tile tileSet[TOTAL_TILES];

    player player;
    enemy enemies[4];

    camera camera;

    camera.box.w = 640;
    camera.box.h = 360;

    setPlayer(&player);
    setEnemies(enemies);

    controller = SDL_JoystickOpen(0);
    if (controller == NULL)
    {
        printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
    }
    else 
    {
        printf("Controller ID: %d, currently connected.\n", SDL_JoystickInstanceID(controller));
    }

    if (setTiles(tileSet))
    {
        while(!quit)
        {
            timer = SDL_GetTicks();

            while (SDL_PollEvent(&e) != 0)  // maybe just event polling ???
            {
                switch (e.type)
                {
                    case SDL_QUIT:
                        quit = true;
                    break;
                    case SDL_KEYDOWN:
                        if (e.key.keysym.sym == SDLK_ESCAPE) quit = true;
                    case SDL_KEYUP:
                        //INPUT_JOYSTICK = false;
                        handleKeyEvents(&e, &player);
                        break;
                    case SDL_JOYAXISMOTION:
                    case SDL_JOYBUTTONDOWN:
                    case SDL_JOYBUTTONUP:
                        //INPUT_JOYSTICK = true;
                        handlePadEvents(&e, &player);
                        break;
                }
            }

            update_player(&player);
            setCamera(&camera.box, player);

            update_enemies(enemies, &player);

            //  test jumping and falling animation logic here   //
            if (player.state == STANDING)
            {
                if (player.xVel) frame++;
                else frame = 0;

                if (frame >> 2 >= 4) frame = 0;

                if (player.running) currentClip = &charClips[(frame >> 2) + 8];
                else currentClip = &charClips[frame >> 2];
            }
            else if (player.jumped) &charClips[4];
            else if (player.state == JUMPING) currentClip = &charClips[5];
            else if (player.state == FALLING) currentClip = &charClips[6];
            else if (player.landed) &charClips[7];
            //      just for testing !!!    //

            // clear screen
            SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
            SDL_RenderClear(renderer);

            // render map
            render_tiles(sheet, tileSet, tileClips, camera.box);

            //  render player
            renderTexture(character, player.x - camera.box.x, player.y - camera.box.y, currentClip, player.facing);

            //  render enemies
            for (int i = 0; i < 4; i++)
            {
                if (!enemies[i].dead)
                {
                    enemyClip = &charClips[12];
                    renderTexture(character, enemies[i].x - camera.box.x , enemies[i].y - camera.box.y, enemyClip, enemies[i].f);
                }
            }

            // put it all together
            SDL_RenderPresent(renderer);

            int delta = SDL_GetTicks() - timer;

            if (delta < TICKS) SDL_Delay(TICKS - delta);
        }
    }

    freeTexture(character);
    freeTexture(sheet);
}

bool initSdl()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow(
            "Platformer Prototype", 
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH, SCREEN_HEIGHT, 
            SDL_WINDOW_SHOWN);
        if( window == NULL )
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
            if (renderer == NULL) 
            {
                printf("Could not create renderer! %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

void closeSdl(SDL_Joystick *controller)
{
    SDL_JoystickClose(controller);
    controller = NULL;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = NULL;
    renderer = NULL;

    IMG_Quit();
    SDL_Quit();
}
