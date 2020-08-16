#include <stdio.h>
#include <stdlib.h>

#include "../source/core.h"
#include "../source/ui/text_lable.h"
#include "../source/ui/button.h"


void callbackPush(void *pObj)
{

}

int main(int argc, char *argv[])
{
    tDE_S_Core *ptDE = tDE_setup_1("ex01",640,480,0);
    if ( !ptDE )
    {
        printf("error sdl\n");
        return 1;
    }

    tDE_S_ObjectBase *pBtn = tDE_ui_createButton(ptDE->m_pRender,320,240,96,32,-1,ptDE->m_pDefaultFont,NULL,NULL,callbackPush);

    
    Uint32 _tick = SDL_GetTicks();
    SDL_bool bLoop = SDL_TRUE;
    while (bLoop)
    {
        Uint32 _delta_tick = _tick - SDL_GetTicks();
        _tick = SDL_GetTicks();
        
        SDL_SetRenderDrawColor(ptDE->m_pRender,0x00,0x00,0xff,0xff);
        SDL_RenderClear(ptDE->m_pRender);

        pBtn->m_fpRender(pBtn,ptDE->m_pRender);
        
        SDL_RenderPresent(ptDE->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            pBtn->m_fpDoEvent(pBtn,&_event);
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
    
    pBtn->m_fpDestory(pBtn);
    tDE_closeCore(ptDE);

    SDL_Quit();
    return 0;
}
