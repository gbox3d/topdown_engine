#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "core.h"

typedef struct __tde_node__ {
    Uint32 m_nID;

    struct __tde_node__ *m_pNext;
    struct __tde_node__ *m_pPrev;
    struct __tde_node__ *m_pParent;
    struct __tde_node__ *m_pChild;
    void *m_pEntity;

} tDE_S_Node;

void tDE_graph_init();
void tDE_graph_close();
Uint32 tDE_graph_add_node(tDE_S_Node *pNode,tDE_S_Node *pParent);
void tDE_graph_remove_node(tDE_S_Node *pNode,tDE_S_Node *pParent);

void tDE_graph_Traverse(tDE_S_Node *pRoot,void (*callback)(void *node,void *),void *param);
tDE_S_Node *tDE_graph_FindNodeByEntity(tDE_S_Node *pRoot,void *pEntity);
tDE_S_Node *tDE_graph_FindNodeById(tDE_S_Node *pRoot, Uint32 nId);

extern tDE_S_Node *g_pRootNode;

#endif