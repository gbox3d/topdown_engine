#include <stdio.h>
#include "../source/core.h"
#include "../source/graph.h"

void callBack(void *ptr, void *param)
{
    int *_data = ((tDE_S_Node *)ptr)->m_pEntity;

    printf("%d\n", *_data);
}

void callBack_free(void *ptr, void *param)
{
    SDL_free(ptr);
}

int main(int argc, char *argv[])
{
    tDE_graph_init();

    int a[5] = {1, 2, 3, 4, 5};
    int ids[5];

    for (int i = 0; i < 5; i++)
    {
        tDE_S_Node *pNode = SDL_calloc(sizeof(tDE_S_Node), 1);
        pNode->m_pEntity = &(a[i]);
        ids[i] = tDE_graph_add_node(pNode, NULL);
    }

    tDE_graph_Traverse(g_pRootNode, callBack, NULL);

    tDE_graph_Traverse(g_pRootNode, callBack_free, NULL);

//엔티티객체로 찾기 
    {
        tDE_S_Node *pnode = tDE_graph_FindNodeByEntity(NULL, &(a[2]));
        printf("find by entity : %d\n", *((int *)(pnode->m_pEntity)));
    }

   
//아이디로 찾기
    {
        tDE_S_Node *pnode = tDE_graph_FindNodeById(NULL, ids[3] );
        printf("id %d find by entity : %d\n",ids[3], *((int *)(pnode->m_pEntity)));
    }

    tDE_graph_close();

    printf("finished ok \n");

    return 0;
}