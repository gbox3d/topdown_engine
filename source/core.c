#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "core.h"

tDE_S_Core *tDE_setup_1(const char *szTitle, int WINDOW_WIDTH, int WINDOW_HEIGHT, Uint32 flags)
{
    tDE_S_Core *pCore = SDL_malloc(sizeof(tDE_S_Core));
    // retutns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return NULL;
    }
    pCore->m_pWin = SDL_CreateWindow(szTitle, // creates a window
                                     SDL_WINDOWPOS_CENTERED,
                                     SDL_WINDOWPOS_CENTERED,
                                     WINDOW_WIDTH, WINDOW_HEIGHT, flags);

    // creates a renderer to render our images
    pCore->m_pRender = SDL_CreateRenderer(pCore->m_pWin, -1, SDL_RENDERER_ACCELERATED);

    //Initialize PNG loading
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return NULL;
    }

    //Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return SDL_FALSE;
    }
    else
    {
        printf("load font success \n");
    }

    //LOAD font file
    pCore->m_pDefaultFont = TTF_OpenFont("../res/nmf.ttf", 12);

    if (!pCore->m_pDefaultFont)
    {
        printf("font file load error");
        printf("SDL_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }

    printf("engine start in : %8d\n", time(NULL));
    srand(time(NULL));

    return pCore;
}

void tDE_closeCore(tDE_S_Core *pCore)
{
    TTF_CloseFont(pCore->m_pDefaultFont);
    SDL_DestroyRenderer(pCore->m_pRender);
    SDL_DestroyWindow(pCore->m_pWin);

    SDL_free(pCore);
}

SDL_Texture *tDE_util_loadTexture(tDE_S_Core *pCore, const char *filename)
{
    // SDL_RenderPresent(renderer);
    SDL_Surface *surface;
    // please provide a path for your image
    surface = IMG_Load(filename);

    if (surface == 0)
    {
        printf("error load file \n");
        return NULL;
    }
    else
    {
        printf("success load file \n");
    }

    // loads image to our graphics hardware memory.
    SDL_Texture *tex = SDL_CreateTextureFromSurface(pCore->m_pRender, surface);

    // clears main-memory
    SDL_FreeSurface(surface);
    return tex;
}