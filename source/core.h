#ifndef __LGE_CORE_H__
#define __LGE_CORE_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

enum {
  TDE_TYPE_BASE = 100,
  TDE_TYPE_UI_TEXTLABLE,
  TDE_TYPE_UI_BUTTON,
  TDE_TYPE_OBJ_BASIC
};

typedef struct _tde_s_base
{
  Uint16 m_nType; //오브잭트 구분하기 위한 형식 식별자 
  SDL_bool m_bVisible;
  Uint16 m_nID;  
  Uint32 m_nNodeId;
  void (*m_fpDestory)(void *pObj);  
  void (*m_fpRender)(void *pObj, SDL_Renderer *pRender);
  void (*m_fpDoEvent)(void *pObj, SDL_Event *pEvt);
  void (*m_fpApply)(void *pObj,Uint32 tick);
  
} tDE_S_ObjectBase;

typedef struct __core
{
    SDL_Window *m_pWin;
    SDL_Renderer *m_pRender;
    TTF_Font *m_pDefaultFont;
    Uint16 m_nScreenWidth;
    Uint16 m_nScreenHeight;
    
} tDE_S_Core;


tDE_S_Core *tDE_setup_1(const char *szTitle,int WINDOW_WIDTH,int WINDOW_HEIGHT,Uint32 flags );
void tDE_closeCore(tDE_S_Core *pCore);

SDL_Texture *tDE_util_loadTexture(tDE_S_Core *pCore, const char *filename);


#endif