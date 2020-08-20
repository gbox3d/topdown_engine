#include <stdio.h>
#include <string.h>
#include "../source/core.h"
#include "../source/tile.h"

tDE_S_Core *g_pEngineCore;
static SDL_bool bLoop = SDL_TRUE;
SDL_bool g_bDrawGrid = SDL_FALSE;

SDL_Rect worldmap_rect = {0, 0, 256, 256};
SDL_Rect tiltPalette_rect = {440, 32, 160, 448};

SDL_Texture *g_pTitleTexture;
Sint16 g_nSelectTileIndex = 0;

Sint16 g_worldMap_Layer[256];
Sint16 g_attrMap_Layer[256];

int main(int argc, char *argv[])
{
    memset(g_worldMap_Layer, -1, sizeof(g_worldMap_Layer));
    memset(g_attrMap_Layer, 0, sizeof(g_worldMap_Layer));

    g_pEngineCore = tDE_setup_1("map editor", 640, 480, 0);
    printf("%4d,%4d\n", g_pEngineCore->m_nScreenWidth, g_pEngineCore->m_nScreenHeight);
    g_pTitleTexture = tDE_util_loadTexture(g_pEngineCore, "../res/gb_ft_rpg_tile_set.png");

    while (bLoop)
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

        // for multiple rendering
        SDL_RenderPresent(g_pEngineCore->m_pRender);

        SDL_Event _event;
        while (SDL_PollEvent(&_event))
        {
            switch (_event.type)
            {
            case SDL_MOUSEMOTION:
            {
                SDL_Point m_pt = {_event.motion.x, _event.motion.y};
                if (_event.button.button == 1)
                {   
                    if (SDL_PointInRect(&m_pt, &worldmap_rect) && !g_bDrawGrid)
                    {
                        tDE_map_put((worldmap_rect.x-m_pt.x)/ 16, (worldmap_rect.y- m_pt.y) / 16, g_nSelectTileIndex, g_worldMap_Layer, 16);
                    }
                }
                else if (_event.button.button == 4 && !g_bDrawGrid) //우 클릭
                {
                    if (SDL_PointInRect(&m_pt, &worldmap_rect))
                    {
                        tDE_map_put((worldmap_rect.x-m_pt.x)/ 16, (worldmap_rect.y- m_pt.y) / 16, -1, g_attrMap_Layer, 16);
                    }
                }
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
                printf("%8d\r", _event.button.button);
                if (_event.button.button == 1) //마우스 좌클릭
                {
                    // int mx, my;
                    // SDL_GetMouseState(&mx, &my);
                    SDL_Point mouse_point = {_event.motion.x, _event.motion.y};
                    //팔래트처리
                    if (SDL_PointInRect(&mouse_point, &tiltPalette_rect))
                    {
                        int _x = (_event.motion.x - tiltPalette_rect.x) / 16;
                        int _y = (_event.motion.y - tiltPalette_rect.y) / 16;
                        g_nSelectTileIndex = _y * 10 + _x;

                        printf("%4d%4d%4d %4d%4d\r", _x, _y, g_nSelectTileIndex,
                               _event.motion.x,
                               _event.motion.y);
                    }
                    else if (SDL_PointInRect(&mouse_point, &worldmap_rect))
                    {
                        int _x = (_event.motion.x - worldmap_rect.x) / 16;
                        int _y = (_event.motion.y - worldmap_rect.y) / 16;

                        //if (g_bDrawGrid) //속성 바꾸기
                        // {
                        //     g_attrMMap_Layer_1[_x + _y * 8] = g_nCurrentAttr;

                        //     printf("%3d,%3d,%3d,%3d\r", _x, _y,
                        //            g_attrMMap_Layer_1[_x + _y * 8],
                        //            g_nCurrentAttr);
                        // }
                        // else
                        {
                            tDE_map_put(_x, _y, g_nSelectTileIndex, g_worldMap_Layer, 16);
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
                            g_worldMap_Layer[_tileIndex] = -1;
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
