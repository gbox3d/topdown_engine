#ifndef __SPRITE_H__
#define __SPRITE_H__

typedef struct _S_sprite_
{
    //배이스 구조체
    tDE_S_ObjectBase m_base;
    SDL_Texture *m_pTexture;
    SDL_Rect m_srcRect;
    SDL_Point m_ptPos;

    void *m_pExtend; //확장기능 

    void (*m_pCallbackMouseDown)(void *);


} tDE_Entity_S_Sprite;

// typedef tDE_UI_S_TextLable *ptDE_UI_S_TextLable;

tDE_S_ObjectBase *tDE_Entity_createSprite(
    int x, int y, Uint16 nID,
    SDL_Rect srcRect,
    SDL_Texture *pTexture,
    void (*pCallbackMouseDown)(void *)
    );

#endif