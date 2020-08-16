#include "../core.h"
#include "sprite.h"

static void _destory(void *pObj)
{
    SDL_free(pObj);
}
static void _render(void *pObj, SDL_Renderer *pRender)
{
    tDE_Entity_S_Sprite *pEnty = pObj;

    SDL_Rect _rt = {pEnty->m_ptPos.x, pEnty->m_ptPos.y, pEnty->m_srcRect.w, pEnty->m_srcRect.h};

    SDL_RenderCopy(pRender, pEnty->m_pTexture, &pEnty->m_srcRect, &_rt);
}

static void _doEvent(void *pObj, SDL_Event *pEvt)
{
    tDE_Entity_S_Sprite *pEnty = pObj;
    switch (pEvt->type)
    {
    case SDL_MOUSEBUTTONDOWN:
        if (pEnty->m_pCallbackMouseDown)
            pEnty->m_pCallbackMouseDown(pEnty);
        break;
    }
}

tDE_S_ObjectBase *tDE_Entity_createSprite(
    int x, int y, Uint16 nID,
    SDL_Rect srcRect,
    SDL_Texture *pTexture,
    void (*pCallbackMouseDown)(void *))
{
    tDE_Entity_S_Sprite *pObj = SDL_malloc(sizeof(tDE_Entity_S_Sprite));

    pObj->m_pTexture = pTexture;
    pObj->m_srcRect = srcRect;

    pObj->m_ptPos.x = x;
    pObj->m_ptPos.y = y;

    pObj->m_base.m_bVisible = SDL_TRUE;
    pObj->m_base.m_fpDestory = _destory;
    pObj->m_base.m_fpRender = _render;
    pObj->m_base.m_fpDoEvent = _doEvent;

    pObj->m_pCallbackMouseDown = pCallbackMouseDown;

    return (void *)pObj;
}