#ifndef DEFINES_H
#define DEFINES_H

extern "C"
{
        #include <os.h>
        #include "SDL/SDL.h"
}

#ifndef DEBUG_MSG
#define DEBUG_MSG//     printf
#endif

#define MESSAGE_TEXT_R           (107)
#define MESSAGE_TEXT_G           (107)
#define MESSAGE_TEXT_B           (71)

#define MESSAGE_BACKGROUND_R     (255)
#define MESSAGE_BACKGROUND_G     (255)
#define MESSAGE_BACKGROUND_B     (207)

int Puz_Min(int a, int b);
int Puz_Max(int a, int b);

void draw_rectangle(SDL_Surface* Surface, Uint32 color, Uint16 x, Uint16 y, Uint16 width, Uint16 height, Uint8 lnpx );

Uint16 get_pixel16( SDL_Surface *surface, int x, int y );
void put_pixel16( SDL_Surface *surface, int x, int y, Uint16 pixel );

#endif
