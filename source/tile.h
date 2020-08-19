#ifndef __TILE_H__
#define __TILE_H__

#include "core.h"


void tDE_putTile(SDL_Renderer *pRenderer, SDL_Texture *pTex,
             Uint16 _x, Uint16 _y, Uint16 _index,
             Uint16 tile_size,
             Uint16 tile_set_width,
             Uint16 zoom);
#endif
