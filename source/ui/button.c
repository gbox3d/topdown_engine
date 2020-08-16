#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "../core.h"
#include "button.h"


static SDL_Color _defaultFillColor = {0xf0,0xf0,0xf0,0xff};
static SDL_Color _defaultBorderColor = {0x8f,0x8f,0x8f,0xff};

static void _destory(void *pObj)
{
  tDE_UI_S_BUTTON *pBtn = pObj;

  // SDL_DestroyTexture(pBtn->m_pLable);
  SDL_free(pBtn);
}

static void _render(void *pObj, SDL_Renderer *pRender)
{
  tDE_UI_S_BUTTON *pBtn = pObj;

  if (pBtn->m_base.m_bVisible)
  {
    //반투명 모드 활성화
    SDL_SetRenderDrawBlendMode(pRender, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(pRender, 255, 255, 255, 255);
    SDL_RenderDrawRect(pRender, &pBtn->m_rtArea);

    SDL_SetRenderDrawColor(pRender, pBtn->m_fillColor.r, pBtn->m_fillColor.g, pBtn->m_fillColor.b,pBtn->m_fillColor.a);
    SDL_RenderFillRect(pRender, &pBtn->m_rtArea);

    SDL_SetRenderDrawColor(pRender, pBtn->m_borderColor.r, pBtn->m_borderColor.g, pBtn->m_borderColor.b,pBtn->m_fillColor.a);
    SDL_RenderDrawRect(pRender, &pBtn->m_rtArea);


    //반투명 모드 비 활성화
    SDL_SetRenderDrawBlendMode(pRender, SDL_BLENDMODE_NONE);
  }
}

static void _doEvent(void *pObj, SDL_Event *pEvt)
{
  tDE_UI_S_BUTTON *pBtn = pObj;

  SDL_Point mousePos;
  mousePos.x = pEvt->motion.x;
  mousePos.y = pEvt->motion.y;

  if (pBtn->m_nFSM == 0)
  {
    switch (pEvt->type)
    {
    case SDL_MOUSEMOTION:
      if (SDL_PointInRect(&mousePos, &pBtn->m_rtArea))
      {
        pBtn->m_fillColor.a = 0x80;
        pBtn->m_nFSM = 1;
      }
      break;
    }
  }
  else if (pBtn->m_nFSM == 1) //사각형 내부진입상태
  {
    switch (pEvt->type)
    {
    case SDL_MOUSEMOTION:
      if (!SDL_PointInRect(&mousePos, &pBtn->m_rtArea))
      {
        pBtn->m_fillColor.a = 0xff;
        pBtn->m_nFSM = 0;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      if (SDL_PointInRect(&mousePos, &pBtn->m_rtArea))
      {
        if (pBtn->m_pCallbackBtnPush != NULL)
        {
          pBtn->m_pCallbackBtnPush(pBtn);
        }
      }
      break;
    }
  }
}

void *tDE_ui_createButton(SDL_Renderer *pRenderer,
                          int x, int y, int w, int h, Uint16 nID,
                          TTF_Font *pFont,
                          SDL_Color *pFillColor, SDL_Color *pBorderColor,
                          void (*pCallbackBtnPush)(void *))
{
  tDE_UI_S_BUTTON *pBtn;
  pBtn = SDL_malloc(sizeof(tDE_UI_S_BUTTON));

  pBtn->m_base.m_nType = TDE_TYPE_UI_BUTTON;
  pBtn->m_base.m_nID = nID;
  pBtn->m_base.m_bVisible = SDL_TRUE;

  pBtn->m_base.m_fpRender = _render;
  pBtn->m_base.m_fpDestory = _destory;
  pBtn->m_base.m_fpDoEvent = _doEvent;

  pBtn->m_rtArea.x = x;
  pBtn->m_rtArea.y = y;
  pBtn->m_rtArea.w = w;
  pBtn->m_rtArea.h = h;

  pBtn->m_nFSM = 0;

  if(pFillColor) pBtn->m_fillColor = *pFillColor;
  else pBtn->m_fillColor = _defaultFillColor;
  

  if(pBorderColor) pBtn->m_borderColor = *pBorderColor;
  else pBtn->m_borderColor = _defaultBorderColor;

  pBtn->m_pCallbackBtnPush = pCallbackBtnPush;

  return pBtn;
}