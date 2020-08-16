#include <stdio.h>
#include <stdlib.h>

#include "../source/core.h"
#include "../source/graph.h"
#include "../source/ui/text_lable.h"
#include "../source/ui/button.h"

tDE_S_Core *g_ptDE;

void callbackPush(void *ptr)
{
    tDE_S_ObjectBase *pObj = ptr;

    if (pObj->m_nID == 1)
    {
        //create
        int x = rand() % 600;
        int y = rand() % 400;

        // tDE_S_ObjectBase *pLabel = tDE_ui_createTextLable(g_ptDE->m_pRender, x, y, -1, L"hello", NULL, NULL, g_ptDE->m_pDefaultFont);
        tDE_S_ObjectBase *pObj = tDE_ui_createButton(g_ptDE->m_pRender, x, y, 96, 32, 2, g_ptDE->m_pDefaultFont, NULL, NULL, callbackPush);

        tDE_S_Node *pNode = SDL_calloc(sizeof(tDE_S_Node), 1); //0으로 초기화하여 할당

        pNode->m_pEntity = pObj;

        tDE_graph_add_node(pNode, NULL);

        printf("add %4d%4d \n", x, y);
    }
    else if(pObj->m_nID == 2)
    {
        tDE_S_Node *pnode = tDE_graph_FindNodeByEntity(NULL,pObj);
        if(pnode) {
            // printf("%d \n",pnode->m_pBody);
            tDE_graph_remove_node(pnode,NULL);
        }
    }
}

void renderGraphCallback(void *ptr,void *param)
{
    tDE_S_Node *pNode = ptr;
    tDE_S_ObjectBase *pObj = pNode->m_pEntity;

    pObj->m_fpRender(pObj, g_ptDE->m_pRender);
}

void eventGraphCallback(void *ptr,void *param)
{
    tDE_S_Node *pNode = ptr;
    tDE_S_ObjectBase *pObj = pNode->m_pEntity;
    pObj->m_fpDoEvent(pObj,param);

}

void clearGraphCallback(void *ptr,void *param)
{
    tDE_S_Node *pNode = ptr;
    tDE_S_ObjectBase *pObj = pNode->m_pEntity;

    printf("free obj : %d \n", pObj->m_nType);

    pObj->m_fpDestory(pObj);
    SDL_free(pNode);
}

int main(int argc, char *argv[])
{
    tDE_S_Core *ptDE = tDE_setup_1("ex01", 640, 480, 0);
    if (!ptDE)
    {
        printf("error sdl\n");
        return 1;
    }
    g_ptDE = ptDE;

    //그래프 초기화
    tDE_graph_init();

    tDE_S_ObjectBase *pBtn = tDE_ui_createButton(ptDE->m_pRender, 320, 240, 96, 32, 1, ptDE->m_pDefaultFont, NULL, NULL, callbackPush);
    

    Uint32 _tick = SDL_GetTicks();
    SDL_bool bLoop = SDL_TRUE;
    while (bLoop)
    {
        Uint32 _delta_tick = _tick - SDL_GetTicks();
        _tick = SDL_GetTicks();

        SDL_SetRenderDrawColor(ptDE->m_pRender, 0x00, 0x00, 0xff, 0xff);
        SDL_RenderClear(ptDE->m_pRender);

        pBtn->m_fpRender(pBtn, ptDE->m_pRender);
        tDE_graph_Traverse(NULL, renderGraphCallback,NULL);

        SDL_RenderPresent(ptDE->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            pBtn->m_fpDoEvent(pBtn, &_event);
            tDE_graph_Traverse(NULL, eventGraphCallback,&_event);


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

    tDE_graph_Traverse(NULL, clearGraphCallback,NULL);

    tDE_graph_close();
    tDE_closeCore(ptDE);

    SDL_Quit();
    return 0;
}
