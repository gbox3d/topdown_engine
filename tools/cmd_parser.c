#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "cmd_parser.h"


#define MAX_TOKEN_SIZE 32

static char pMsgBuf[256];

int doTokenize(char *szBuf, char szBufToken[8][512])
{
  char *szpTemp;
  // char *pNextToken = NULL;
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

void parseCmd(char *_szCmd)
{
  static char szCmd[1024];
  static char szTokens[8][512];

  strcpy(szCmd, _szCmd);

  // printf("token count %s\n", szCmd);
  int _numToken = doTokenize(szCmd, szTokens);
  printf("token count %d\n", _numToken);

  if (strcmp(szTokens[0], "quit") == 0)
  {
    SDL_Event evt;
    evt.type = SDL_QUIT;
    evt.quit.timestamp = SDL_GetTicks();
    SDL_PushEvent(&evt);
  }  
  else if (strcmp(szTokens[0], "save") == 0)
  {
    //save file.map
    char *pFileName = szTokens[1];    

    static char pMsg[32];
    strcpy(pMsg, "save");
    strcpy(pMsg + 16, pFileName);
    SDL_Event evt;
    evt.type = SDL_USEREVENT;
    evt.user.data1 = pMsg;
    evt.user.timestamp = SDL_GetTicks();
    SDL_PushEvent(&evt);
  }
  else if (strcmp(szTokens[0], "load") == 0)
  {
    char *pFileName = szTokens[1];
    static char pMsg[32];
    strcpy(pMsg, "load");
    strcpy(pMsg + 16, pFileName);
    SDL_Event evt;
    evt.type = SDL_USEREVENT;
    evt.user.data1 = pMsg;
    evt.user.timestamp = SDL_GetTicks();
    SDL_PushEvent(&evt);

    // SDL_RWops *rw = SDL_RWFromFile(pFileName, "rb");
    // SDL_RWread(rw, g_worldMap_Layer[0], sizeof(Uint16), 64);
    // SDL_RWclose(rw);
  }
  else if (strcmp(szTokens[0], "new") == 0)
  {
    // memset(g_worldMap_Layer[0],-1,128);
    static char *pMsg = "new";
    SDL_Event evt;
    evt.type = SDL_USEREVENT;
    evt.user.data1 = pMsg;
    evt.user.timestamp = SDL_GetTicks();
    SDL_PushEvent(&evt);
  }
  else if (strcmp(szTokens[0], "brush") == 0) //brush change (attr)
  {
    SDL_Event evt;
    Uint16 cmd=0;
    Uint16 value=0;
    if (strcmp(szTokens[1], "change") == 0)
    {
      cmd = 1;
      value = atoi(szTokens[2]);
    }
    else 
    {
      evt.user.data1 = NULL;
      evt.user.code = CMD_PARSER_CODE;
    }
    
    strcpy(pMsgBuf, szTokens[0]);

    memcpy(pMsgBuf + 16, &cmd, sizeof(Uint16));
    memcpy(pMsgBuf + 18, &value, sizeof(Uint16));

    // printf("%d,%d\n",cmd, *(Uint16*)pMsgBuf+18);

    
    evt.type = SDL_USEREVENT;
    evt.user.data1 = pMsgBuf;
    evt.user.code = CMD_PARSER_CODE;
    evt.user.timestamp = SDL_GetTicks();

    SDL_PushEvent(&evt);
  }
  else if (strcmp(szTokens[0], "version") == 0)
  {
    strcpy(pMsgBuf, szTokens[0]);
    Uint16 _version = 1;
    memcpy(pMsgBuf+16, &_version,sizeof(Uint16));

    SDL_Event evt;
    evt.type = SDL_USEREVENT;
    evt.user.data1 = pMsgBuf;
    evt.user.code = CMD_PARSER_CODE;
    evt.user.timestamp = SDL_GetTicks();
    SDL_PushEvent(&evt);

  }
  else if (strcmp(szTokens[0], "setText") == 0)
  {
    static char szBuf[128];
    szBuf[0] = 0x00;
    for(int i=1;i<_numToken;i++)
    {
      strcat(szBuf," ");
      strcat(szBuf,szTokens[i]);    
    }
    strcpy(pMsgBuf,szTokens[0]);
    strcpy(pMsgBuf+16, szBuf);

    SDL_Event evt;
    evt.type = SDL_USEREVENT;
    evt.user.data1 = pMsgBuf;
    evt.user.code = CMD_PARSER_CODE;
    evt.user.timestamp = SDL_GetTicks();
    SDL_PushEvent(&evt);
  }
  else 
  {
    strcpy(pMsgBuf, szTokens[0]);
    
    SDL_Event evt;
    evt.type = SDL_USEREVENT;
    evt.user.data1 = pMsgBuf;
    evt.user.code = CMD_PARSER_CODE;
    evt.user.timestamp = SDL_GetTicks();
    SDL_PushEvent(&evt);
  }
}
