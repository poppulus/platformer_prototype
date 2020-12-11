#include <SDL2/SDL.h>

const short TILE_SIZE = 32;

typedef struct tile
{
    unsigned char type, set[4];

    SDL_Rect box;
} tile;

enum map_tiles
{
    NONE, 
    NORMAL,
    PLATFORM_FILL,
    NORMAL_L_CORNER,
    NORMAL_R_CORNER,
    NORMAL_GRASS,
};

const unsigned int map[12] = {  // level collision, in reverse because of checking bits
    0b00000000000000000000,
    0b00000000000000000000,
    0b00000000000101000000,
    0b00000001010000000000,
    0b00010100000000000000,
    0b10000000000000000000,
    0b01000000000000000000,
    0b00010100000000000000,
    0b00000001001000000000,
    0b00000000000010010000,
    0b00000000000000000100,
    0b11111111111111111111
};

const unsigned char tile_set[4] = {
    NORMAL, NORMAL, 
    PLATFORM_FILL, PLATFORM_FILL
}, 
tile_set_2[4] = {
    NORMAL_L_CORNER, NORMAL_R_CORNER, 
    NORMAL_L_CORNER, NORMAL_R_CORNER
};

