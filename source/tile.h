#ifndef __TILE_H__
#define __TILE_H__

#include "core.h"

void tDE_putTile(SDL_Renderer *pRenderer, SDL_Texture *pTex,
             Uint16 _x, Uint16 _y, Uint16 _index,
             Uint16 tile_size,
             Uint16 tile_set_width,
             Uint16 zoom);

void tDE_map_put(Uint16 x, Uint16 y, Sint16 nTile, Sint16 *map, Uint16 map_wsize);

void tDE_map_drawall(SDL_Renderer *pRender, SDL_Texture *pTileSet, int tile_size,
               int tileset_width,
               int zoom,
               int x, int y, int map_wsize, Sint16 *map);


SDL_bool tDE_map_load(const char *filename, Sint16 *map[2],int data_size);
SDL_bool tDE_map_save(const char *filename, Sint16 *map[2],int data_size);
               
#endif
