#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../core.h"
#include "input_text.h"

static void _destory(void *_pObj)
{
  S_INPUT_TEXT *pObj = _pObj;

  if (pObj->m_pTextTxture)
  {
    SDL_DestroyTexture(pObj->m_pTextTxture);
  }

  SDL_free(pObj);
}
static void _render(void *_pObj, SDL_Renderer *pRender)
{
  //...
  S_INPUT_TEXT *pObj = _pObj;

  //배경
  SDL_SetRenderDrawColor(pRender,
                         pObj->m_bgColor.r,
                         pObj->m_bgColor.g,
                         pObj->m_bgColor.b,
                         pObj->m_bgColor.a);
  SDL_RenderFillRect(pRender, &pObj->m_Rect);

  //전경 텍스트
  if(strlen(pObj->m_szBuf) > 0)
  {
    // SDL_Color _whiteColor = {0xff, 0xff, 0xff, 0xff};
    // SDL_Color _blackColor = {0, 0, 0, 0x00};
    SDL_Surface *textSurface = TTF_RenderText_Solid(pObj->m_pFont,
                                                       pObj->m_szBuf,
                                                       pObj->m_fgColor);

    pObj->m_pTextTxture = SDL_CreateTextureFromSurface(pRender, textSurface);    
    
    pObj->m_textSize.x = textSurface->w;
    pObj->m_textSize.y = textSurface->h;
    SDL_FreeSurface(textSurface);
  }

  if(pObj->m_pTextTxture)
  {
    SDL_Rect _dstRect = {pObj->m_Rect.x,pObj->m_Rect.y,pObj->m_textSize.x,pObj->m_textSize.y};
    SDL_RenderCopy(pRender,pObj->m_pTextTxture,NULL,&_dstRect);
    SDL_DestroyTexture(pObj->m_pTextTxture);
    pObj->m_pTextTxture = NULL;
  }
}

static void _doEvent(void *_pObj, SDL_Event *evt)
{
  S_INPUT_TEXT *pObj = _pObj;
  if (evt->type == SDL_TEXTINPUT)
  {
    strcat(pObj->m_szBuf, evt->text.text);
    printf("%s  \r", pObj->m_szBuf);
  }
  else if (evt->type == SDL_KEYDOWN)
  {
    if (evt->key.keysym.sym == SDLK_BACKSPACE)
    {
      int _len = strlen(pObj->m_szBuf);
      pObj->m_szBuf[_len - 1] = 0x00;
      // printf("%s  \r", pObj->m_szBuf);

      SDL_Event evt;
      evt.type = SDL_TEXTINPUT;
      evt.text.text[0] = 0x00;
      SDL_PushEvent(&evt);
    }
  }
}

S_INPUT_TEXT *myui_createInputText(SDL_Renderer *pRenderer,
                                   int x, int y, int w, int h,
                                   Uint16 nID,
                                   TTF_Font *pFont,
                                   SDL_Color fgColor,
                                   SDL_Color bgColor)
{
  S_INPUT_TEXT *pObj = SDL_malloc(sizeof(S_INPUT_TEXT));

  pObj->m_base.m_nType = MYUI_TEXT_INPUT;
  pObj->m_base.m_nID = nID;

  pObj->m_Rect.x = x;
  pObj->m_Rect.y = y;
  pObj->m_Rect.w = w;
  pObj->m_Rect.h = h;

  pObj->m_szBuf[0] = 0x00;

  pObj->m_pFont = pFont;
  pObj->m_pTextTxture = NULL;

  pObj->m_fgColor = fgColor;
  pObj->m_bgColor = bgColor;

  pObj->m_base.m_fpRender = _render;
  pObj->m_base.m_fpDestory = _destory;
  pObj->m_base.m_fpDoEvent = _doEvent;

  return pObj;
}
