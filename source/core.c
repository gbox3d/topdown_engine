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
        return NULL;
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

    printf("engine start in : %8ld\n", time(NULL));
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

void tDE_core_doCmdInput(SDL_Event *_event)
{
    static char strBuf[128];
    static Uint16 nInputFSM = 0;

    if (nInputFSM == 0)
    {
        if (_event->type == SDL_KEYDOWN)
        {
            if (_event->key.keysym.sym == SDLK_RETURN)
            {
                printf("input msg : \n");
                // SDL_StartTextInput();
                nInputFSM = 1;
            }
        }
    }
    else if (nInputFSM == 1)
    {
        if (_event->type == SDL_KEYDOWN)
        {
            if (_event->key.keysym.sym == SDLK_RETURN)
            {
                // printf("input msg : \n");
                // SDL_StartTextInput();
                SDL_Event evt;
                evt.type = TDE_EVENT_CMD_INPUT;
                evt.user.timestamp = SDL_GetTicks();
                // evt.user.data1 = strBuf;
                static char szTemp[128];
                strcpy(szTemp, strBuf);
                evt.user.data1 = szTemp;
                SDL_PushEvent(&evt);
                // printf("\n%s\n", strBuf);
                //함수로 만들어 놓기
                // processCmd(strBuf, &bLoop);
                strBuf[0] = 0x00;
                // SDL_StopTextInput();
                nInputFSM = 0;
            }
            // printf(">%s\r",strBuf);
        }
        else if(_event->type == SDL_TEXTINPUT)
        {
            strcat(strBuf, _event->text.text);
        }
        
    }

    
}

int tDE_util_doTokenize(char *szBuf, char (*szBufToken)[32])
{
    //change 10
    //[change,10]
    char *szpTemp;
    //   char *pNextToken = NULL;
    const char *pszDelimiter = " ";
    szpTemp = strtok(szBuf, pszDelimiter);

    int _nTokenIndex = 0;
    while (szpTemp != NULL)
    {
        strcpy(szBufToken[_nTokenIndex], szpTemp);
        _nTokenIndex++;
        szpTemp = strtok(NULL, pszDelimiter);
    }
    return _nTokenIndex;
}


