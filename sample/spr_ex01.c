#include <stdio.h>
#include <stdlib.h>

#include "../source/core.h"
#include "../source/entity/sprite.h"

void onMouseDown(void *ptr)
{
    tDE_S_ObjectBase *pSpr = ptr;

    printf("click \n");

    
}

int main(int argc, char *argv[])
{
    tDE_S_Core *pEngineCore = tDE_setup_1("sprite exam01", 640, 480, 0);

    SDL_Texture *pSprTex = tDE_util_loadTexture(pEngineCore, "../res/tank/Spritesheet/sheet_tanks.png");

//x="671" y="0" width="75" height="70"
    SDL_Rect _rt = {671,0,75,70};
    tDE_S_ObjectBase *pSpr = tDE_Entity_createSprite(320,200,-1,_rt,pSprTex,onMouseDown);

    SDL_bool bLoop = SDL_TRUE;
    while (bLoop)
    {
        SDL_SetRenderDrawColor(pEngineCore->m_pRender, 0x00, 0x00, 0xff, 0xff);
        SDL_RenderClear(pEngineCore->m_pRender);

        pSpr->m_fpRender(pSpr,pEngineCore->m_pRender);


        SDL_RenderPresent(pEngineCore->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            pSpr->m_fpDoEvent(pSpr,&_event);
            
            switch (_event.type)
            {
            case SDL_KEYDOWN:
                // printf("%d \n", _event.key.keysym.scancode);
                break;
            case SDL_QUIT:
                bLoop = SDL_FALSE;
                break;
            default:
                break;
            }
        }
    }

    pSpr->m_fpDestory(pSpr);

    SDL_DestroyTexture(pSprTex);

    tDE_closeCore(pEngineCore);

    return 0;
}