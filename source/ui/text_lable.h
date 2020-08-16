#ifndef __TEXT_LABLE_H__
#define __TEXT_LABLE_H__

typedef struct _text_lable
{
  //배이스 구조체 
  tDE_S_ObjectBase m_base;

  TTF_Font *m_pFont;
  SDL_Texture *m_pLableTxture;
  
  SDL_Rect m_Rect;
  Uint16 m_nFSM;

  SDL_Color m_fgColor;
  SDL_Color m_bgColor; 

  

} tDE_UI_S_TextLable;

typedef tDE_UI_S_TextLable* ptDE_UI_S_TextLable; 

tDE_S_ObjectBase *tDE_ui_createTextLable(SDL_Renderer *pRenderer,
                         int x, int y, Uint16 nID,
                         const Uint16 *text,
                         SDL_Color *pFgColor,SDL_Color *pBgColor,
                         TTF_Font *pFont);

#endif