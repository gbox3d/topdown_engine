#include <stdio.h>
#include <string.h>
#include "../source/core.h"
#include "../source/tile.h"
#include "cmd_parser.h"

tDE_S_Core *g_pEngineCore;
static SDL_bool bLoop = SDL_TRUE;
SDL_bool g_bDrawGrid = SDL_FALSE;

SDL_Rect worldmap_rect = {0, 0, 256, 256};
SDL_Rect tiltPalette_rect = {440, 32, 160, 448};

SDL_Texture *g_pTitleTexture;
Sint16 g_nSelectTileIndex = 0;
Sint16 g_nCurrentAttr = 0;

Sint16 g_worldMap_Layer[256];
Sint16 g_attrMap_Layer[256];

void render()
{
    // clears the screen
    SDL_SetRenderDrawColor(g_pEngineCore->m_pRender, 0x00, 0x00, 0x00, 0xff);
    SDL_RenderClear(g_pEngineCore->m_pRender);

    //팔래트
    SDL_RenderCopy(g_pEngineCore->m_pRender, g_pTitleTexture, NULL, &tiltPalette_rect);

    //선택타일 그리기
    tDE_putTile(g_pEngineCore->m_pRender, g_pTitleTexture, 16, 0,
                g_nSelectTileIndex, 16, 10, 2);

    //월드멥그리기
    SDL_SetRenderDrawColor(g_pEngineCore->m_pRender, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderDrawRect(g_pEngineCore->m_pRender, &worldmap_rect);

    tDE_map_drawall(g_pEngineCore->m_pRender, g_pTitleTexture, 16, 10, 1,
                    0, 0,
                    16,
                    g_worldMap_Layer);
    //그리드 그리기
    if (g_bDrawGrid)
    {
        SDL_Renderer *pRender = g_pEngineCore->m_pRender;
        for (int ix = 0; ix < 16; ix++)
        {
            SDL_SetRenderDrawColor(pRender, 0xff, 0xff, 0x00, 0xff);
            SDL_RenderDrawLine(pRender, ix * 16, 0, ix * 16, 256);
        }
        for (int iy = 0; iy < 16; iy++)
        {
            SDL_SetRenderDrawColor(pRender, 0xff, 0xff, 0x00, 0xff);
            SDL_RenderDrawLine(pRender, 0, iy * 16, 256, iy * 16);
        }

        //속성멥 그리기
        for (int i = 0; i < 256; i++)
        {
            SDL_SetRenderDrawBlendMode(pRender, SDL_BLENDMODE_BLEND);
            Sint16 _index = g_attrMap_Layer[i];
            SDL_Rect _rt = {(i % 16) * 16, (i / 16) * 16, 16, 16};
            if (_index == 1) //벽
            {
                SDL_SetRenderDrawColor(pRender, 0xff, 0xff, 0xff, 0x80);

                SDL_RenderFillRect(pRender, &_rt);
            }
            else if (_index == 2) //비상구
            {
                SDL_SetRenderDrawColor(pRender, 0x00, 0xff, 0x00, 0x80);

                SDL_RenderFillRect(pRender, &_rt);
            }
            else if (_index == 3) //숨겨진문
            {
                SDL_SetRenderDrawColor(pRender, 0xff, 0x00, 0x00, 0x80);

                SDL_RenderFillRect(pRender, &_rt);
            }
            else if (_index == 4) //switch
            {
                SDL_SetRenderDrawColor(pRender, 0xff, 0xff, 0x00, 0x80);

                SDL_RenderFillRect(pRender, &_rt);
            }

            SDL_SetRenderDrawBlendMode(pRender, SDL_BLENDMODE_NONE);
        }
    }

    // for multiple rendering
    SDL_RenderPresent(g_pEngineCore->m_pRender);
}

void processEvent()
{
    static char szBuf[128];
    static int nInputFSM = 0;
    SDL_Event _event;
    while (SDL_PollEvent(&_event))
    {
        switch (_event.type)
        {
        case SDL_MOUSEMOTION:
        {
            // printf("%8d\n", _event.button.button);

            SDL_Point m_pt = {_event.motion.x, _event.motion.y};
            if (_event.button.button == 1)
            {

                if (SDL_PointInRect(&m_pt, &worldmap_rect))
                {
                    // printf("drag %d\n",m_pt.x);
                    if (!g_bDrawGrid)
                        tDE_map_put((m_pt.x - worldmap_rect.x) / 16, (m_pt.y - worldmap_rect.y) / 16, g_nSelectTileIndex, g_worldMap_Layer, 16);
                    else
                        tDE_map_put((m_pt.x - worldmap_rect.x) / 16, (m_pt.y - worldmap_rect.y) / 16, g_nCurrentAttr, g_attrMap_Layer, 16);
                }
            }
            else if (_event.button.button == 4 && !g_bDrawGrid) //우 클릭
            {
                if (SDL_PointInRect(&m_pt, &worldmap_rect))
                {
                    tDE_map_put((m_pt.x - worldmap_rect.x) / 16, (m_pt.y - worldmap_rect.y) / 16, -1, g_attrMap_Layer, 16);
                }
            }
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        {
            SDL_Point mouse_point = {_event.motion.x, _event.motion.y};
            // printf("%8d\n", _event.button.button);
            if (_event.button.button == 1) //마우스 좌클릭
            {
                // int mx, my;
                // SDL_GetMouseState(&mx, &my);

                //팔래트처리
                if (SDL_PointInRect(&mouse_point, &tiltPalette_rect))
                {
                    int _x = (_event.motion.x - tiltPalette_rect.x) / 16;
                    int _y = (_event.motion.y - tiltPalette_rect.y) / 16;
                    g_nSelectTileIndex = _y * 10 + _x;
                }
                else if (SDL_PointInRect(&mouse_point, &worldmap_rect))
                {
                    int _x = (_event.motion.x - worldmap_rect.x) / 16;
                    int _y = (_event.motion.y - worldmap_rect.y) / 16;
                    if (!g_bDrawGrid)
                        tDE_map_put(_x, _y, g_nSelectTileIndex, g_worldMap_Layer, 16);
                    else
                        tDE_map_put(_x, _y, g_nCurrentAttr, g_attrMap_Layer, 16);
                }
            }
            else if (_event.button.button == 3) //마우스 우클릭
            {
                if (SDL_PointInRect(&mouse_point, &worldmap_rect))
                {
                    int _x = (_event.motion.x - worldmap_rect.x) / 16;
                    int _y = (_event.motion.y - worldmap_rect.y) / 16;
                    if(!g_bDrawGrid)
                        tDE_map_put(_x, _y, -1, g_worldMap_Layer, 16);
                    if(g_bDrawGrid)
                        tDE_map_put(_x, _y, 0, g_attrMap_Layer, 16);
                }
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            // printf("%d %d\n", _event.key.keysym.scancode);
            switch (nInputFSM)
            {
            case 0: //대기상태
            {
                if (_event.key.keysym.sym == SDLK_RETURN)
                {
                    printf("input cmd => \n");
                    nInputFSM = 1; //입력 상태로 전이
                }
                else if (_event.key.keysym.sym == SDLK_g)
                {
                    g_bDrawGrid = !g_bDrawGrid;
                }
                else if (_event.key.keysym.sym == SDLK_1)
                {
                    parseCmd("brush change 1");
                }
                else if (_event.key.keysym.sym == SDLK_2)
                {
                    parseCmd("brush change 2");
                }
                else if (_event.key.keysym.sym == SDLK_3)
                {
                    parseCmd("brush change 3");
                }
                else if (_event.key.keysym.sym == SDLK_4)
                {
                    parseCmd("brush change 4");
                }
                else if (_event.key.keysym.sym == SDLK_5)
                {
                    parseCmd("brush change 5");
                }
                else if (_event.key.keysym.sym == SDLK_0)
                {
                    parseCmd("brush change 0");
                }
            }
            break;
            case 1: //입력 상태
            {
                if (_event.key.keysym.sym == SDLK_RETURN)
                {
                    nInputFSM = 0; //대기 상태로 전이
                    parseCmd(szBuf);
                    szBuf[0] = 0x00; //문자열 클리어
                }
                else if (_event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    int _len = strlen(szBuf);
                    szBuf[_len - 1] = 0x00;
                    printf("%s  \r", szBuf);
                    fflush(stdout);
                }
            }
            break;
            default:
                break;
            }
        }
        break;
        case SDL_TEXTINPUT:
        {
            if (nInputFSM == 1)
            {
                strcat(szBuf, _event.text.text);
                printf("%s  \r", szBuf);
                fflush(stdout);
            }
        }
        break;
        case SDL_USEREVENT:
        {
            if (!strcmp(_event.user.data1, "version"))
            {
                void *pData = (char *)_event.user.data1 + 16;
                printf("version %d\n", *(Uint16 *)pData);
            }
            else if (!strcmp(_event.user.data1, "brush"))
            {
                char *pData = (char *)_event.user.data1 + 16;

                Uint16 cmd = *(Uint16 *)pData;
                Uint16 value = *(Uint16 *)(pData + 2);

                if (cmd == 1) //change
                {
                    g_nCurrentAttr = value;
                }

                printf("attr %d\n", g_nCurrentAttr);
            }
            else if (strcmp(_event.user.data1, "save") == 0)
            {
                char *pFileName = ((char *)_event.user.data1 + 16);
                Sint16 *map[2] = {g_worldMap_Layer, g_attrMap_Layer};
                tDE_map_save(pFileName, map,256);

                printf("save file name %s \n", pFileName);
            }
            else if (strcmp(_event.user.data1, "load") == 0)
            {
                char *pFileName = ((char *)_event.user.data1 + 16);
                Sint16 *map[2] = {g_worldMap_Layer, g_attrMap_Layer};
                tDE_map_load(pFileName, map,256);
            }
            else if (strcmp(_event.user.data1, "new") == 0)
            {
                memset(g_worldMap_Layer, -1, sizeof(Sint16) * 256);
                memset(g_attrMap_Layer, 0, sizeof(Sint16) * 256);
            }
            else
            {
                printf("unknown : %s \n", _event.user.data1);
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

int main(int argc, char *argv[])
{
    memset(g_worldMap_Layer, -1, sizeof(g_worldMap_Layer));
    memset(g_attrMap_Layer, 0, sizeof(g_worldMap_Layer));

    g_pEngineCore = tDE_setup_1("map editor", 640, 480, 0);
    printf("%4d,%4d\n", g_pEngineCore->m_nScreenWidth, g_pEngineCore->m_nScreenHeight);
    g_pTitleTexture = tDE_util_loadTexture(g_pEngineCore, "../res/gb_ft_rpg_tile_set.png");

    while (bLoop)
    {
        render();
        processEvent();
    }

    SDL_DestroyTexture(g_pTitleTexture);

    tDE_closeCore(g_pEngineCore);

    SDL_Quit();
    return 0;
}
