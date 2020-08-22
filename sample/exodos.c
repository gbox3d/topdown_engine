#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "../source/core.h"
#include "../source/tile.h"
#include "../tools/cmd_parser.h"

tDE_S_Core *g_pEngineCore;

#define MAP_WIDTH 16
#define MAP_SIZE 256
Sint16 g_WorldMap[MAP_SIZE];
Sint16 g_AttrLayer[MAP_SIZE];
Sint16 g_ObjLayer[MAP_SIZE];

SDL_Texture *g_pTileSet;
// SDL_Texture *g_pTileSet2;

SDL_Texture *g_pTextTxture = NULL;
SDL_Rect g_rectText; //텍스트 텍스춰 크기

SDL_Point g_PlayerPos;

int g_nGameLogicFsm = 0;

typedef struct __stage
{
  char *fileName; //멥파일 이름
  SDL_Point m_respwanPos; //리스폰 위치
  char *introMsg; //스테이지 오프닝 대사 

} S_Stage_Info;

S_Stage_Info g_stage_info[] = {
    {"../tools/l1.map", {1, 1}, "여기는 조용한 곳입니다."},
    {"../tools/l2.map", {1, 2}, "여기는 더욱 조용한 곳입니다."},
    {"end", {0, 0}, "탈출 대성공!"}};
int g_nCurrentStageNumber = 0;

SDL_bool scancodeKeyTable[1024] = {
    SDL_FALSE,
};

void doGameLogic()
{  
  memset(g_ObjLayer, 0, 256 * sizeof(Uint16));

  switch (g_nGameLogicFsm)
  {
  case 0:
    printf("start game? (press key)\n");
    g_nGameLogicFsm = 1; //wait
    g_nCurrentStageNumber = 0;
    break;  
  case 1:
    if(scancodeKeyTable[SDL_SCANCODE_SPACE])
    {
      scancodeKeyTable[SDL_SCANCODE_SPACE] = SDL_FALSE;
      g_nGameLogicFsm = 5;      
    }
  break;
  case 5:    
    {    
      S_Stage_Info *pStgInfo = &g_stage_info[g_nCurrentStageNumber];
      printf("%s \n", pStgInfo->introMsg);
      static char strCmd[256];
      sprintf(strCmd,"setText %s",pStgInfo->introMsg);
      parseCmd(strCmd);

      //게임시작 준비
      Sint16 *map[2] = {g_WorldMap, g_AttrLayer};
      tDE_map_load(pStgInfo->fileName, map,256);
      //플레이어 정보 초기화
      g_PlayerPos = pStgInfo->m_respwanPos;
      g_nGameLogicFsm = 10;
    }
    break;
  case 10:
  {
    SDL_Point oldPos = g_PlayerPos;
    if (scancodeKeyTable[SDL_SCANCODE_LEFT]) //left
    {
      scancodeKeyTable[SDL_SCANCODE_LEFT] = SDL_FALSE;
      g_PlayerPos.x--;
    }
    if (scancodeKeyTable[SDL_SCANCODE_RIGHT]) //right
    {
      scancodeKeyTable[SDL_SCANCODE_RIGHT] = SDL_FALSE;
      g_PlayerPos.x++;
    }
    if (scancodeKeyTable[SDL_SCANCODE_UP]) //up
    {
      scancodeKeyTable[SDL_SCANCODE_UP] = SDL_FALSE;
      g_PlayerPos.y--;
    }
    if (scancodeKeyTable[SDL_SCANCODE_DOWN]) //down
    {
      scancodeKeyTable[SDL_SCANCODE_DOWN] = SDL_FALSE;
      g_PlayerPos.y++;
    }
    //충돌처리
    Sint16 _attr = g_AttrLayer[g_PlayerPos.x + (g_PlayerPos.y * MAP_WIDTH)];
    printf("%d \r",_attr);
    //1 벽,3 닫힌문 2열린문 , 4 도어오픈 스위치
    if (_attr == 1 || _attr == 3) //벽
    {
      g_PlayerPos = oldPos;
    }
    else if (_attr == 2) //비상구 도착 
    {      
      //다음 스테이지로
      g_nGameLogicFsm = 20;
    }
    else if (_attr == 4)
    {
      for (int i = 0; i < MAP_SIZE; i++)
      {
        if (g_AttrLayer[i] == 3)
        {
          g_WorldMap[i] = 7;  //열린문 타일 번호 
          g_AttrLayer[i] = 2; //비상구속성으로 변환 
          break;
        }
      }
    }
    //오브잭트 레이어 처리    
    g_ObjLayer[g_PlayerPos.y * MAP_WIDTH + g_PlayerPos.x] = 1;    
  }
  break;
  case 20: //next
  {
    g_nCurrentStageNumber++;
    S_Stage_Info *pInfoStg = &g_stage_info[g_nCurrentStageNumber];    
    //마지막 스테이지 이면
    if (!strcmp("end", pInfoStg->fileName))
    {
      static char strCmd[256];
      sprintf(strCmd,"setText %s",pInfoStg->introMsg);
      parseCmd(strCmd);
      g_nGameLogicFsm = 100;
    }
    else
    {
      g_nGameLogicFsm = 5;
    }
  }
  break;
  case 100:
  {
    printf("game over\n");
    g_nGameLogicFsm = 999;
  }
  break;
  }
}

void renderGame()
{
  SDL_SetRenderDrawColor(g_pEngineCore->m_pRender, 0x00, 0x00, 0xff, 0xff);
  SDL_RenderClear(g_pEngineCore->m_pRender);

  //월드멥 랜더링
  tDE_map_drawall(g_pEngineCore->m_pRender, g_pTileSet, 16, 10, 1,
                    0, 0,
                    MAP_WIDTH,
                    g_WorldMap);

  //오브잭트 레이어 랜더링
  {
    Sint16 *map = g_ObjLayer;
    for (int i = 0; i < MAP_SIZE; i++)
    {
      Sint16 _index = map[i];
      if (_index != -1)
      {
        if (_index == 1)
        {
          //캐릭터 그리기
          int x = i % MAP_WIDTH;
          int y = i / MAP_WIDTH;
          tDE_putTile(g_pEngineCore->m_pRender, g_pTileSet, x, y, 190,
                  MAP_WIDTH, 10, 1);
        }
      }
    }
  }

  //text 랜더링
  {
    if (g_pTextTxture)
    {
      g_rectText.x = 0;
      g_rectText.y = 400;

      SDL_RenderCopy(g_pEngineCore->m_pRender, g_pTextTxture, NULL, &g_rectText);
    }
  }

  SDL_RenderPresent(g_pEngineCore->m_pRender);
}

SDL_bool bLoop = SDL_TRUE;

void doEvent()
{
  static char szBuf[64];
  static Uint16 nInputFSM = 0;
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_KEYUP:
    {
      switch (nInputFSM)
      {
      case 0: //대기상태
      {       
        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
          //bLoop = SDL_FALSE;
          parseCmd("quit");
        }
        else if (event.key.keysym.sym == SDLK_RETURN)
        {
          printf("input cmd => \n");
          nInputFSM = 1; //입력 상태로 전이
        }
        else 
        {
          scancodeKeyTable[event.key.keysym.scancode] = SDL_TRUE;  
        }
      }
      break;
      case 1:
      {
        //텍스트 입력상태 
        if (event.key.keysym.sym == SDLK_RETURN)
        {
          nInputFSM = 0; //대기 상태로 전이
          // printf("\n%s\n",szBuf);
          //cmd parser
          {
            parseCmd(szBuf);
          }

          szBuf[0] = 0x00; //문자열 클리어
        }
        else if (event.key.keysym.sym == SDLK_BACKSPACE)
        {
          int _len = strlen(szBuf);
          szBuf[_len - 1] = 0x00;
          printf("%s  \r", szBuf);
          fflush(stdout);
        }
      }
      break;
      }
    }
    break;
    case SDL_TEXTINPUT:
    {
      if (nInputFSM == 1)
      {
        strcat(szBuf, event.text.text);
        printf("%s  \r", szBuf);
      }
    }
    break;
    case SDL_QUIT:
      bLoop = SDL_FALSE;
      break;
    case SDL_USEREVENT:
    {
      if (!strcmp("setText", event.user.data1))
      {
        char *pText = (char *)event.user.data1 + 16;
        
        //이전에 텍스춰 데이터가있으면 지우기 
        if (g_pTextTxture)
          SDL_DestroyTexture(g_pTextTxture);

        if(strlen(pText) > 0)
        {
          g_pTextTxture = tDE_util_createTextTexture(g_pEngineCore->m_pRender,
                                                   g_pEngineCore->m_pDefaultFont,
                                                   pText, &g_rectText);
        }

        
      }
    }
    break;
    default:
      break;
    }
  }
}

int main(int argc, char **argv)
{
  g_PlayerPos.x = 1;
  g_PlayerPos.y = 1;

  g_pEngineCore = tDE_setup_1("game", 640, 480, 0);  
  g_pTileSet = tDE_util_loadTexture(g_pEngineCore, "../res/gb_ft_rpg_tile_set.png");
  memset(g_WorldMap, -1, 256 * sizeof(Uint16));
  parseCmd("setText 시작하려면 슾이스바~");
  
  while (bLoop)
  {
    //게임로직
    doGameLogic();

    //랜더링
    renderGame();

    //이벤트 처리
    doEvent();
  }

  if (g_pTextTxture)
  {
    SDL_DestroyTexture(g_pTextTxture);
  }

  SDL_DestroyTexture(g_pTileSet);
  tDE_closeCore(g_pEngineCore);

  return 0;
}