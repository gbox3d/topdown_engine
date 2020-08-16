#include "graph.h"
#include <memory.h>

tDE_S_Node *g_pRootNode;
static Uint32 __nodeIDGenerator = 10000;

void tDE_graph_init()
{
    g_pRootNode = SDL_malloc(sizeof(tDE_S_Node));
    __nodeIDGenerator = 10000;

    //0으로 초기화
    memset(g_pRootNode, 0, sizeof(tDE_S_Node));
}

void tDE_graph_close()
{
    SDL_free(g_pRootNode);
}

Uint32 tDE_graph_add_node(tDE_S_Node *pNode, tDE_S_Node *pParent)
{
    // tDE_S_Node *_pParent = pParent;
    pNode->m_nID = __nodeIDGenerator++;

    if (!pParent)
    {
        pParent = g_pRootNode;

        if (!pParent->m_pChild)
        {
            pParent->m_pChild = pNode;
            pNode->m_pParent = pParent;
        }
        else
        {
            //기존에 헤더노드가 존재하면 새로운 해더로 옹립하고 기존껀 뒤로 보내기
            tDE_S_Node *_pChild = pParent->m_pChild;

            pParent->m_pChild = pNode;
            pNode->m_pParent = pParent;
            pNode->m_pNext = _pChild;
            _pChild->m_pPrev = pNode;
        }
    }

    return pNode->m_nID;
}

void tDE_graph_remove_node(tDE_S_Node *pNode, tDE_S_Node *pParent)
{
    if (!pParent)
    {
        pParent = g_pRootNode;
    }
    // tDE_S_Node *_pParent = pParent;
    tDE_S_Node *prev = pNode->m_pPrev;
    tDE_S_Node *next = pNode->m_pNext;
    if (!prev)
    {
        //만약 헤더라면
        if (next)
        {
            pParent->m_pChild = next;
            next->m_pPrev = NULL;
        }
        else
        {
            //마지막 원소이면
            pParent->m_pChild = NULL;
        }
    }
    else
    {
        //중간에 잇는 값이면
        prev->m_pNext = next;

        if (next)
            next->m_pPrev = prev;
    }
}

void tDE_graph_Traverse(tDE_S_Node *pRoot, void (*callback)(void *node, void *), void *param)
{

    if (!pRoot)
    {
        pRoot = g_pRootNode;
    }

    if (pRoot->m_pChild)
    {
        tDE_graph_Traverse(pRoot->m_pChild, callback, param);

        tDE_S_Node *_pHead = pRoot->m_pChild;
        while (_pHead)
        {
            void *_next = _pHead->m_pNext;
            callback(_pHead, param);
            _pHead = _next;
        }
    }
    return;
}

tDE_S_Node *tDE_graph_FindNodeByEntity(tDE_S_Node *pRoot, void *pEntity)
{
    if (!pRoot)
    {
        pRoot = g_pRootNode;
    }

    if (pRoot->m_pChild)
    {
        void *_pEntity = tDE_graph_FindNodeByEntity(pRoot->m_pChild, pEntity);

        if (_pEntity)
            return _pEntity;
        else
        {
            tDE_S_Node *_pHead = pRoot->m_pChild;
            while (_pHead)
            {
                void *_next = _pHead->m_pNext;
                if (_pHead->m_pEntity == pEntity)
                    return _pHead;

                _pHead = _next;
            }
        }
    }

    return NULL;
}


tDE_S_Node *tDE_graph_FindNodeById(tDE_S_Node *pRoot, Uint32 nId)
{
    if (!pRoot)
    {
        pRoot = g_pRootNode;
    }

    if (pRoot->m_pChild)
    {
        void *_pEntity = tDE_graph_FindNodeById(pRoot->m_pChild, nId);

        if (_pEntity)
            return _pEntity;
        else
        {
            tDE_S_Node *_pHead = pRoot->m_pChild;
            while (_pHead)
            {
                void *_next = _pHead->m_pNext;
                if (_pHead->m_nID == nId)
                    return _pHead;
                _pHead = _next;
            }
        }
    }

    return NULL;
}