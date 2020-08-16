#include <stdio.h>
#include <stdlib.h>

#include "../source/core.h"
#include "../source/ui/text_lable.h"

Uint32 callBackLoop(Uint32 interval,void *param)
{
    tDE_UI_S_TextLable *plb = param;

    plb->m_Rect.x = rand() % 600;
    plb->m_Rect.y = rand() % 400; 


    return interval;
}


int main(int argc, char *argv[])
{
    tDE_S_Core *ptDE = tDE_setup_1("ex01",640,480,0);
    if ( !ptDE )
    {
        printf("error sdl\n");
        return 1;
    }

    
    tDE_S_ObjectBase *pLable = tDE_ui_createTextLable(ptDE->m_pRender,200,100,-1,L"hello",NULL,NULL,ptDE->m_pDefaultFont);

    SDL_AddTimer(1000,callBackLoop,pLable);

    SDL_bool bLoop = SDL_TRUE;
    while (bLoop)
    {
        SDL_SetRenderDrawColor(ptDE->m_pRender,0x00,0x00,0xff,0xff);
        SDL_RenderClear(ptDE->m_pRender);

        pLable->m_fpRender(pLable,ptDE->m_pRender);

        SDL_RenderPresent(ptDE->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            switch (_event.type)
            {
            case SDL_KEYDOWN:
                printf("%d \n", _event.key.keysym.scancode);
                break;
            case SDL_QUIT:
                bLoop = SDL_FALSE;
                break;
            default:
                break;
            }
        }
    }

    pLable->m_fpDestory(pLable);
    tDE_closeCore(ptDE);

    SDL_Quit();
    return 0;
}
