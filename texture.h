#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

typedef struct texture texture;

struct texture                  // what to do with this?
{
    SDL_Texture *mTexture;

    int mWidth, mHeight;

    //bool (*loadTexture)(char* [], SDL_Texture*);
}; 

void freeTexture(texture *text)
{
    if (text->mTexture != NULL)
    {
        SDL_DestroyTexture(text->mTexture);
        text->mTexture = NULL;

        text->mWidth = 0;
        text->mHeight = 0;
    }
}

void renderTexture(texture *text, int x, int y, SDL_Rect *clip, const SDL_RendererFlip flip)
{
    SDL_Rect renderQuad = {x, y, text->mWidth, text->mHeight};

    if (clip != NULL) 
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(renderer, text->mTexture, clip, &renderQuad, 0, NULL, flip);
}

SDL_Texture *loadTexture(char path[16])
{
    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path);

    if (loadedSurface == NULL) printf("could not load image! %s\n", IMG_GetError());
    else 
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        
        if (newTexture == NULL) printf("could not load optimised surface! %s\n", IMG_GetError());

        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

void setCharClips(SDL_Rect clips[])
{
    // 0 - 3    walking animation
    clips[0].x = 6;
    clips[0].y = 72;
    clips[0].w = 18;
    clips[0].h = 24;

    clips[1].x = 38;
    clips[1].y = 72;
    clips[1].w = 18;
    clips[1].h = 24;

    clips[2].x = 70;
    clips[2].y = 72;
    clips[2].w = 18;
    clips[2].h = 24;

    clips[3].x = 102;
    clips[3].y = 72;
    clips[3].w = 18;
    clips[3].h = 24;

    //  4 - 7   jumping frames
    clips[4].x = 135;
    clips[4].y = 72;
    clips[4].w = 18;
    clips[4].h = 24;

    clips[5].x = 166;
    clips[5].y = 72;
    clips[5].w = 18;
    clips[5].h = 24;

    clips[6].x = 198;
    clips[6].y = 72;
    clips[6].w = 18;
    clips[6].h = 24;

    clips[7].x = 230;
    clips[7].y = 72;
    clips[7].w = 18;
    clips[7].h = 24;

    // 8 - 11   running frames
    clips[8].x = 454;
    clips[8].y = 72;
    clips[8].w = 18;
    clips[8].h = 24;

    clips[9].x = 486;
    clips[9].y = 72;
    clips[9].w = 18;
    clips[9].h = 24;

    clips[10].x = 518;
    clips[10].y = 72;
    clips[10].w = 18;
    clips[10].h = 24;

    clips[11].x = 551;
    clips[11].y = 72;
    clips[11].w = 18;
    clips[11].h = 24;

    // 12 - 15  enemy walking
    clips[12].x = 7;
    clips[12].y = 114;
    clips[12].w = 18;
    clips[12].h = 14;

    clips[13].x = 40;
    clips[13].y = 114;
    clips[13].w = 18;
    clips[13].h = 14;

    clips[14].x = 71;
    clips[14].y = 114;
    clips[14].w = 18;
    clips[14].h = 14;

    clips[15].x = 103;
    clips[15].y = 114;
    clips[15].w = 18;
    clips[15].h = 14;
}
