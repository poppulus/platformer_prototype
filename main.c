#include "game.h"

int main()
{
    SDL_Joystick* gGameController;

    texture charTexture;
    texture sheetTexture;

    SDL_Rect charClips[16];
    SDL_Rect tileClips[5];

    if (initSdl())
    {
        charTexture.mTexture = loadTexture("assets/characters.png");
        sheetTexture.mTexture = loadTexture("assets/sheet.png");

        if (charTexture.mTexture == NULL) printf("Failed to load character image!\n");
        else if (sheetTexture.mTexture == NULL) printf("Failed to load sheet image!\n");
        else 
        {
            setCharClips(charClips);

            tileClips[NONE].x = 192;
            tileClips[NONE].y = 80;
            tileClips[NONE].w = 16;
            tileClips[NONE].h = 16;

            tileClips[NORMAL].x = 176;
            tileClips[NORMAL].y = 0;
            tileClips[NORMAL].w = 16;
            tileClips[NORMAL].h = 16;

            tileClips[PLATFORM_FILL].x = 176;
            tileClips[PLATFORM_FILL].y = 16;
            tileClips[PLATFORM_FILL].w = 16;
            tileClips[PLATFORM_FILL].h = 16;

            tileClips[NORMAL_L_CORNER].x = 160;
            tileClips[NORMAL_L_CORNER].y = 0;
            tileClips[NORMAL_L_CORNER].w = 16;
            tileClips[NORMAL_L_CORNER].h = 16;

            tileClips[NORMAL_R_CORNER].x = 192;
            tileClips[NORMAL_R_CORNER].y = 0;
            tileClips[NORMAL_R_CORNER].w = 16;
            tileClips[NORMAL_R_CORNER].h = 16;

            gameLoop(gGameController, &charTexture, &sheetTexture, charClips, tileClips);
        }
    }
    else printf("Failed to initialize SDL!\n");

    closeSdl(gGameController);

    return 0;
}
