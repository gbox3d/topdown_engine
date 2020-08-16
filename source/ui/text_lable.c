#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../core.h"
#include "text_lable.h"

static void _destory(void *_pObj)
{
  tDE_UI_S_TextLable *pObj = _pObj;

  SDL_DestroyTexture(pObj->m_pLableTxture);
  SDL_free(pObj);
}

static void _render(void *_pObj, SDL_Renderer *pRender)
{
  tDE_UI_S_TextLable *pObj = _pObj;
  if (pObj->m_base.m_bVisible)
  {
    SDL_SetRenderDrawBlendMode(pRender, SDL_BLENDMODE_BLEND);

    SDL_RenderCopy(pRender, pObj->m_pLableTxture, NULL, &pObj->m_Rect);

    SDL_SetRenderDrawBlendMode(pRender, SDL_BLENDMODE_NONE);
  }
}

tDE_S_ObjectBase *tDE_ui_createTextLable(SDL_Renderer *pRenderer,
                                           int x, int y, Uint16 nID,
                                           const Uint16 *text,
                                           SDL_Color *pFgColor, SDL_Color *pBgColor,
                                           TTF_Font *pFont)
{
  tDE_UI_S_TextLable *pObj = SDL_malloc(sizeof(tDE_UI_S_TextLable));

  pObj->m_base.m_bVisible = SDL_TRUE;

  pObj->m_base.m_nType = TDE_TYPE_UI_TEXTLABLE;
  pObj->m_base.m_nID = nID;

  pObj->m_Rect.x = x;
  pObj->m_Rect.y = y;

  //라벨생성
  {

    SDL_Color _whiteColor = {0xff, 0xff, 0xff, 0xff};
    if (pFgColor)
    {
      _whiteColor = *pFgColor;
    }
    SDL_Color _blackColor = {0, 0, 0, 0x00};
    if (pBgColor)
    {
      _blackColor = *pBgColor;
    }

    SDL_Surface *textSurface = TTF_RenderUNICODE_Solid(pFont, text, _whiteColor);
    pObj->m_pLableTxture = SDL_CreateTextureFromSurface(pRenderer, textSurface);

    pObj->m_Rect.w = textSurface->w;
    pObj->m_Rect.h = textSurface->h;

    SDL_FreeSurface(textSurface);
  }

  pObj->m_base.m_fpRender = _render;
  pObj->m_base.m_fpDestory = _destory;

  return (void *)pObj;
}
