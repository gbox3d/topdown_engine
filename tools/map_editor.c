#include "../source/core.h"
#include "../source/tile.h"

tDE_S_Core *g_pEngineCore;

SDL_Texture *g_pTitleTexture;
Sint16 g_nSelectTileIndex = 0;
Sint16 g_worldMap_Layer_1[64];

void processCmd(char *_szCmd, SDL_bool *bLoop)
{
    static char szCmd[32];
    static char szTokens[8][32];

    strcpy(szCmd, _szCmd);

    printf("token count %s\n", szCmd);
    int _numToken = tDE_util_doTokenize(szCmd, szTokens);

    printf("token count %d\n", _numToken);

    if (strcmp(szTokens[0], "quit") == 0)
    {
        *bLoop = SDL_FALSE;
    }
    else if (strcmp(szTokens[0], "setTile") == 0)
    {
        //setTile x y index
        int _x = SDL_atoi(szTokens[1]);
        int _y = SDL_atoi(szTokens[2]);
        int _index = SDL_atoi(szTokens[3]);

        g_worldMap_Layer_1[(_y * 8) + _x] = _index;
    }
    else if (strcmp(szTokens[0], "save") == 0)
    {
        //save file.map
        char *pFileName = szTokens[1];
        SDL_RWops *rw = SDL_RWFromFile(pFileName, "wb");
        SDL_RWwrite(rw, g_worldMap_Layer_1, sizeof(Uint16), 64);
        SDL_RWclose(rw);
    }
    else if (strcmp(szTokens[0], "load") == 0)
    {
        char *pFileName = szTokens[1];
        SDL_RWops *rw = SDL_RWFromFile(pFileName, "rb");
        SDL_RWread(rw, g_worldMap_Layer_1, sizeof(Uint16), 64);
        SDL_RWclose(rw);
    }
}

int main(int argc, char *argv[])
{
    for (int i = 0; i < 64; i++)
    {
        g_worldMap_Layer_1[i] = -1;
    }

    g_pEngineCore = tDE_setup_1("map editor", 640, 480, 0);
    printf("%4d,%4d\n", g_pEngineCore->m_nScreenWidth, g_pEngineCore->m_nScreenHeight);
    g_pTitleTexture = tDE_util_loadTexture(g_pEngineCore, "../res/dungeontiles.png");

    static SDL_bool bLoop = SDL_TRUE;

    while (bLoop)
    {
        // clears the screen
        SDL_SetRenderDrawColor(g_pEngineCore->m_pRender, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderClear(g_pEngineCore->m_pRender);

        {
            SDL_Rect _dstRT = {440, 96, 48 * 4, 72 * 4};
            SDL_RenderCopy(g_pEngineCore->m_pRender, g_pTitleTexture, NULL, &_dstRT);
        }

        {
            tDE_putTile(g_pEngineCore->m_pRender, g_pTitleTexture, 14, 0, g_nSelectTileIndex);
        }

        {
            for (int i = 0; i < 64; i++)
            {
                int _index = g_worldMap_Layer_1[i];
                if (_index != -1)
                {
                    tDE_putTile(g_pEngineCore->m_pRender, g_pTitleTexture, i % 8, i / 8, _index);
                }
            }
        }

        // for (int i = 0; i < 64; i++)
        // {
        //     if (g_worldMap_Layer_1[i] != -1)
        //     {
        //         putTile(g_pEngineCore->m_pRender, g_pTitleTexture, i % 8, i / 8, g_worldMap_Layer_1[i]);
        //     }
        // }

        // putTile(g_pRenderer,g_pTitleTexture,1,1,0);
        // putTile(g_pRenderer,g_pTitleTexture,2,1,1);

        // for multiple rendering
        SDL_RenderPresent(g_pEngineCore->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            switch (_event.type)
            {
            case SDL_MOUSEBUTTONDOWN:
            {
                printf("%8d\r", _event.button.button);
                if (_event.button.button == 1) //마우스 좌클릭
                {
                    //팔래트처리
                    {
                        int _x = (_event.motion.x - 440) / 32;
                        int _y = (_event.motion.y - 100) / 32;

                        if ((_x >= 0 && _y >= 0) && (_x < 6 && _y < 9))
                        {
                            g_nSelectTileIndex = _y * 6 + _x;
                        }

                        // printf("%4d%4d\r", _x, _y);
                    }
                    //월드멥처리
                    {
                        int _x = (_event.motion.x) / 32;
                        int _y = (_event.motion.y) / 32;

                        if (_x < 8 && _y < 8)
                        {
                            int _tileIndex = _y * 8 + _x;
                            // printf("%8d \r",_tileIndex);
                            g_worldMap_Layer_1[_tileIndex] = g_nSelectTileIndex;
                            printf("%4d%4d%4d\r", _x, _y, _tileIndex);
                        }
                    }
                }
                else if (_event.button.button == 3) //마우스 우클릭
                {
                    {
                        int _x = (_event.motion.x) / 32;
                        int _y = (_event.motion.y) / 32;

                        if (_x < 8 && _y < 8)
                        {
                            int _tileIndex = _y * 8 + _x;
                            g_worldMap_Layer_1[_tileIndex] = -1;
                        }
                    }
                }
            }
            break;
            case SDL_QUIT:
                bLoop = SDL_FALSE;
                break;
            default:
                break;
            }
        }
    }

    SDL_DestroyTexture(g_pTitleTexture);

    tDE_closeCore(g_pEngineCore);

    SDL_Quit();
    return 0;
}
