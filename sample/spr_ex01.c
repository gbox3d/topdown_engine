#include <stdio.h>
#include <stdlib.h>

#include "../source/core.h"
#include "../source/graph.h"
#include "../source/entity/sprite.h"

void onMouseDown(void *ptr)
{
    tDE_S_ObjectBase *pSpr = ptr;

    printf("click %d \n",pSpr->m_nID);
}

void onRenderCallback(void *ptr,void *_render)
{
    tDE_S_Node *pNode = ptr;
    tDE_S_ObjectBase *pObj = pNode->m_pEntity;
    SDL_Renderer *pRenderer = _render;
    pObj->m_fpRender(pObj,pRenderer);
}

void onDestroyCallback(void *ptr,void *param)
{
    tDE_S_Node *pNode = ptr;
    tDE_S_ObjectBase *pObj = pNode->m_pEntity;
    pObj->m_fpDestory(pObj);
}

void onEventCallback(void *ptr,void *param)
{
    tDE_S_Node *pNode = ptr;
    tDE_S_ObjectBase *pEnty = pNode->m_pEntity;

    pEnty->m_fpDoEvent(pEnty,(SDL_Event *)param);
    
}

int main(int argc, char *argv[])
{
    tDE_graph_init();

    tDE_S_Core *pEngineCore = tDE_setup_1("sprite exam01", 640, 480, 0);

    SDL_Texture *pSprTex = tDE_util_loadTexture(pEngineCore, "../res/tank/Spritesheet/sheet_tanks.png");

    //x="671" y="0" width="75" height="70"
    SDL_Rect _rt = {671, 0, 75, 70};
    for(int i=0;i<10;i++)
    {
        int x = rand()%640;
        int y=rand()%480;

        tDE_S_ObjectBase *pSpr = tDE_Entity_createSprite(x, y, -1, _rt, pSprTex,
                                                         onMouseDown,
                                                         NULL);
        tDE_S_Node *pNode = SDL_calloc(sizeof(tDE_S_Node),1);
        pSpr->m_nID = tDE_graph_add_node(pNode,NULL);
        pNode->m_pEntity = pSpr;
    }

    SDL_bool bLoop = SDL_TRUE;
    while (bLoop)
    {
        SDL_SetRenderDrawColor(pEngineCore->m_pRender, 0x00, 0x00, 0xff, 0xff);
        SDL_RenderClear(pEngineCore->m_pRender);

        // pSpr->m_fpRender(pSpr,pEngineCore->m_pRender);
        tDE_graph_Traverse(NULL,onRenderCallback,pEngineCore->m_pRender);

        SDL_RenderPresent(pEngineCore->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            // pSpr->m_fpDoEvent(pSpr, &_event);
            tDE_graph_Traverse(NULL,onEventCallback,&_event);

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

    // pSpr->m_fpDestory(pSpr);
    tDE_graph_Traverse(NULL,onDestroyCallback,NULL);

    SDL_DestroyTexture(pSprTex);

    tDE_closeCore(pEngineCore);

    tDE_graph_close();

    return 0;
}