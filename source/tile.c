#include "tile.h"

void tDE_putTile(SDL_Renderer *pRenderer, SDL_Texture *pTex,Uint16 _x,Uint16 _y, Uint16 _index)
{
  SDL_Rect _tmpDstRt;
  _tmpDstRt.x = _x *32;
  _tmpDstRt.y = _y *32;
  _tmpDstRt.w = 32;
  _tmpDstRt.h = 32;
  SDL_Rect _tmpSrcRt;
  _tmpSrcRt.x = (_index % 6) * 8;
  _tmpSrcRt.y = (_index / 6) * 8;
  _tmpSrcRt.w = 8;
  _tmpSrcRt.h = 8;

  SDL_RenderCopy(pRenderer, pTex, &_tmpSrcRt, &_tmpDstRt);
}
