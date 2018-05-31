#ifndef HW5_GRAPH_H
#define HW5_GRAPH_H

#define INF 10000000
#define NIL -10000000
#define NO 0

typedef struct
{
    int node_a; //방향이 없으므로.
    int node_b;
} Edge;

typedef struct
{
    int nodeNum;
    char **nodes; //읽어온 순서대로 nodes[index] = 도시명
    int **w;      //w[nodes][nodes]값을 가짐 , 인접노드에 대한 정보 역할도 얘가 하도록(인접노드 없으면 NO 로 초기화)
    //Edge edges;
    Edge edges[200];
    int edgeNum;
} Graph;

#endif

