#ifndef __INPUT_TEXT_H__
#define __INPUT_TEXT_H__

typedef struct _input_text
{
  //배이스 구조체 
  LGE_S_ObjectBase m_base;

  TTF_Font *m_pFont;
  SDL_Texture *m_pTextTxture;
  
  SDL_Rect m_Rect;
  Uint16 m_nFSM;

  SDL_Point m_textSize;  

  SDL_Color m_fgColor;
  SDL_Color m_bgColor; 

  char m_szBuf[256];

} ;

 S_INPUT_TEXT *myui_createInputText(SDL_Renderer *pRenderer,
                         int x, int y,int w,int h,
                         Uint16 nID,
                         TTF_Font *pFont,
                         SDL_Color fgColor,
                         SDL_Color bgColor
                         );

#endif