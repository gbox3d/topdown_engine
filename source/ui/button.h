#ifndef __BUTTON_H__
#define __BUTTON_H__

typedef struct __s_button_
{
  //배이스 구조체 
  tDE_S_ObjectBase m_base;
  
  SDL_Rect m_rtArea;

  Uint16 m_nFSM;
  SDL_Color m_fillColor;
  SDL_Color m_borderColor;

  void (*m_pCallbackBtnPush)(void *);

} tDE_UI_S_BUTTON;

void *tDE_ui_createButton(SDL_Renderer *pRenderer,
                       int x, int y, int w, int h, Uint16 nID,
                       TTF_Font *pFont,
                       SDL_Color *pColor,SDL_Color *pColorHover,
                       void (*pCallbackBtnPush)(void *));

#endif