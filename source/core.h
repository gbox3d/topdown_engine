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

enum {
  TDE_EVENT_BASE = SDL_USEREVENT + 1000,
  TDE_EVENT_CMD_INPUT,
  TDE_EVENT_GAME_LOOP
};


typedef struct _tde_s_base
{
  Uint16 m_nType; //오브잭트 구분하기 위한 형식 식별자 
  SDL_bool m_bVisible;
  Uint16 m_nID;  
  void (*m_fpDestory)(void *pObj);  
  void (*m_fpRender)(void *pObj, SDL_Renderer *pRender);
  void (*m_fpDoEvent)(void *pObj, SDL_Event *pEvt);
  void (*m_fpApply)(void *pObj,Uint32 tick);
  
} tDE_S_ObjectBase;

// typedef tDE_S_ObjectBase tDE_S_EntityBase;

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
int tDE_util_doTokenize(char *szBuf, char (*szBufToken)[32]);
void tDE_core_doCmdInput(SDL_Event *_event);

//sprite parser 
typedef struct __s_sheet_data
{
  char szName[32];
  SDL_Rect m_area;
} tDE_S_SheetData;
SDL_bool tDE_util_parseSheet(char *szStr, tDE_S_SheetData *pData);

SDL_Texture *tDE_util_createTextTexture(SDL_Renderer *pRenderer,
                                        TTF_Font *pFont, const char *text,
                                        SDL_Rect *ptextRect);


#endif
