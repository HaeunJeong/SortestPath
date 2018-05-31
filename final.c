#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>
#include "HW5_Heap.h"
#include "HW5_Graph.h"

typedef struct
{
    Graph *graph;
    int **distance; //distance[source][node]: source에서 노드까지의 최단거리
    int **pi;
} Path;

Graph *init_Graph(FILE *fp);
Path *init_all_source(Graph *graph); //새로 path를 만들고, 거기에 값을 다 할당.
Path *init_for_floyd(Graph *graph);
void Relax(Path *path, int source, int u, int v);
//void print_shortest_path(Path *path);
void print_shortest_path(Path *path, Graph *graph);

Path *dijkstra(Graph *graph);
Path *bellman_ford(Graph *graph);
Path *floyd_warshall(Graph *graph);

int min(int a, int b);
void free_memory_forPath(Path *path);
void free_memory_forGraph(Graph *graph);

int main(int argc, char *argv[]){

    int nodeNum;
    if (argc == 1)
    {
        printf("Put your input file name\n");
        exit(1);
    }
    FILE *fp = fopen(argv[1], "rt");
    //FILE *fp = fopen("hw5..data", "rt");
    if (fp == NULL)
    {
        printf("NO file\n");
        exit(1);
    }

    Graph *graph = init_Graph(fp);
    nodeNum = graph->nodeNum;

    // Path *dijkstra_path = dijkstra(graph);
    // print_shortest_path(dijkstra_path);

    Path *bellman_ford_path = bellman_ford(graph);
    print_shortest_path(bellman_ford_path, graph);

    Path *floyd_path = floyd_warshall(graph);
    print_shortest_path(floyd_path, graph);

   // free_memory_forPath(dijkstra_path);
    free_memory_forPath(bellman_ford_path);
    free_memory_forPath(floyd_path);
    free_memory_forGraph(graph);

   // free(dijkstra_path);
    free(bellman_ford_path);
    free(floyd_path);
    free(graph);

    
    return 0;
}

Path *init_all_source(Graph *graph)
{
    int nodeNum = graph->nodeNum;

    Path *path = (Path *)malloc(sizeof(Path));
    path->graph = graph; //===================================================================== 이걸 빼먹었었다!!!

    //distance[source][nodes]를 INF로 초기화
    path->distance = (int **)malloc(sizeof(int **) * nodeNum); //row길이. 총 몇개 노드를 수용할건지
    for (int i = 0; i < nodeNum; i++)
    {
        path->distance[i] = (int *)malloc(sizeof(int *) * nodeNum); //distance[source_index][]의 값의 크기
        memset(path->distance[i], 0, sizeof(int) * nodeNum);        //초기화

        for (int j = 0; j < nodeNum; j++)
        {
            if (i == j)
            {
                path->distance[i][j] = 0; //d[s]=0의 효과를 가짐
                continue;
            }
            path->distance[i][j] = INF;
            //printf("source: %d, dest: %d, value: %d\n", i,j,path->distance[i][j]);
        }
    }

    //pi[source][nodes]를 NIL로 초기화
    path->pi = (int **)malloc(sizeof(int **) * nodeNum); //row길이. 총 몇개 노드를 수용할건지
    for (int i = 0; i < nodeNum; i++)
    {
        path->pi[i] = (int *)malloc(sizeof(int *)); //distance[source_index][]의 값의 크기
        memset(path->pi[i], 0, sizeof(int));        //초기화

        for (int j = 0; j < nodeNum; j++)
        {
            if (i == j)
            {
                path->pi[i][j] = NO; //pi[s]=0의 효과를 가짐
                continue;
            }
            path->pi[i][j] = NIL;
        }
    }
    //printf("init_distance  완료");

    return path;
}
Path *init_for_floyd(Graph *graph)
{
    int nodeNum = graph->nodeNum;

    Path *path = (Path *)malloc(sizeof(Path));
    path->graph = graph;

    //distance[source][nodes]를 INF로 초기화
    path->distance = (int **)malloc(sizeof(int **) * nodeNum); //row길이. 총 몇개 노드를 수용할건지
    for (int i = 0; i < nodeNum; i++)
    {
        path->distance[i] = (int *)malloc(sizeof(int *) * nodeNum); //distance[source_index][]의 값의 크기
        memset(path->distance[i], 0, sizeof(int) * nodeNum);        //초기화

        for (int j = 0; j < nodeNum; j++)
        {
            if (i == j)
            {
                path->distance[i][j] = 0; //d[s]=0의 효과를 가짐
                continue;
            }
            else if (path->graph->w[i][j] == NO)
            {
                path->distance[i][j] = INF;
            }
            else
            {
                path->distance[i][j] = path->graph->w[i][j];
            }
        }
    }

    //pi[source][nodes]를 NIL로 초기화
    path->pi = (int **)malloc(sizeof(int **) * nodeNum); //row길이. 총 몇개 노드를 수용할건지
    for (int i = 0; i < nodeNum; i++)
    {
        path->pi[i] = (int *)malloc(sizeof(int *)); //distance[source_index][]의 값의 크기
        memset(path->pi[i], 0, sizeof(int));        //초기화

        for (int j = 0; j < nodeNum; j++)
        {
            if (i == j)
            {
                path->pi[i][j] = NO; //pi[s]=0의 효과를 가짐
                continue;
            }
            path->pi[i][j] = NIL;
        }
    }

    return path;
}

//void print_shortest_path(Path *path)
void print_shortest_path(Path *path, Graph *graph)
{
    int nodeNum = graph->nodeNum;

    printf("\n\t");
    for(int i=0; i<nodeNum; i++){
        printf("%s\t", graph->nodes[i]);
    }
    //printf("\n");
    for (int source = 0; source < nodeNum; source++)
    {
        printf("%s\t", graph->nodes[source]);
        for (int dest = 0; dest < nodeNum; dest++)
        {
            printf("%d\t", path->distance[source][dest]);
        }
        printf("\n");
    }

    for (int source = 0; source < nodeNum; source++)
    {
        for (int dest = 0; dest < nodeNum; dest++)
        {
            printf("%d\t", path->distance[source][dest]);
        }
        printf("\n");
    }
    printf("\n*****************************************************************************\n");
}

Path *dijkstra(Graph *graph){

    time_t startTime = 0, endTime = 0;
    startTime = clock();

    //MinHeap Q;
    MinHeap Q;


    Path *dijkstra_path = init_all_source(graph);
    dijkstra_path->graph = graph;

    int nodeNum = graph->nodeNum;
    int u, v = 0; //부모 자식, node index

    for (int source = 0; source < nodeNum; source++)
    { //source 를 바꿔가며 다 돌려서 모든 노드에서의 값을 구할거다.

        Q = Heap_init(); //큐 초기화

        for (int node = 0; node < nodeNum; node++){ //하나씩 노드를 힙 어레이에 집어넣는다.
            Heap_Insert(&Q, node, dijkstra_path->distance[source][node]);
        }
        while (!Heap_IsEmpty(&Q)){//큐에서 하나씩 빼준다
            u = Heap_remove(&Q);//가장 작은값
            
            for (int v = 0; v < nodeNum; v++){
                if (dijkstra_path->graph->w[u][v] != NO){
                    Relax(dijkstra_path, source, u, v);
                    Heap_Update(&Q, v, dijkstra_path->distance[source][v]); //여기에서 값 업데이트랑, Heapify 를 같이 해준다.
                    //nodeIndex가 v 인 친구를 찾아서, 바뀐 값으로 업데이트
                }
            }
            // printf("after %d removed, remain heap is ", u); //u는 nodeName
            // for (int i = 0; i < Q->size; i++)
            // {
            //     printf("%d -> ", Q->node[i].nodeIndex);
            // }
            // printf("next remove value is %d\n", Q->node[0].distance);
        }
        free(&Q);
        //printf("heap is empty\n");
    }

    endTime = clock();

    printf("It took %f seconds to compute shortest path between cities with Dijkstra's algorithm as follows.\n", (float)endTime - startTime);

    return dijkstra_path;
}
void Relax(Path *path, int source, int u, int v)
{
    //printf("u: %d, v: %d\n",u,v);

    if (path->distance[source][v] > (path->distance[source][u]) + (path->graph->w[u][v]))
    {
        //printf("u: %d, v: %d \t %d > %d \t", u, v,path->distance[source][v], (path->distance[source][u]) + (path->graph->w[u][v]));
        path->distance[source][v] = path->distance[source][u] + path->graph->w[u][v];
        path->pi[source][v] = u;
        //printf(" -> %d \n", path->distance[source][v]);
    }
}

Path *bellman_ford(Graph *graph)
{

    time_t startTime = 0, endTime = 0;
    startTime = clock();

    Path *bellman_ford_path = init_all_source(graph);
    bellman_ford_path->graph = graph;

    int nodeNum = graph->nodeNum;
    int edgeNum = graph->edgeNum;

    int u, v;

    for (int source = 0; source < nodeNum; source++)
    { //모든 노드를 source로 해서
        for (int i = 0; i < nodeNum - 1; i++)
        { //1~|V|-1 만큼 연산
            //모든 엣지에 대해 Relax 연산
            for (int edge = 0; edge < edgeNum; edge++)
            {
                u = bellman_ford_path->graph->edges[edge].node_a;
                v = bellman_ford_path->graph->edges[edge].node_b;
                Relax(bellman_ford_path, source, u, v);
                Relax(bellman_ford_path, source, v, u); //============이렇게 Relax두번 해서 해결해도 될런지 모르겠다...
                                                        //printf("u: %d, v: %d \t %d > %d \t", u, v,bellman_ford_path->distance[source][v], (bellman_ford_path->distance[source][u]) + (bellman_ford_path->graph->w[u][v]));
            }
        }
    }

    endTime = clock();

    printf("It took %f seconds to compute shortest path between cities with Bellman Ford's algorithm as follows.\n", (float)endTime - startTime);

    return bellman_ford_path;
}

Path *floyd_warshall(Graph *graph)
{

    time_t startTime = 0, endTime = 0;
    startTime = clock();

    Path *floyd_warshall_path = init_for_floyd(graph);

    floyd_warshall_path->graph = graph;
    int **distance = floyd_warshall_path->distance;

    int nodeNum = graph->nodeNum;
    int edgeNum = graph->edgeNum;

    int n = 1;

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (k != 0)
                    distance[i][j] = min(distance[i][k] + distance[k][j], distance[i][j]);
                if (n != nodeNum)
                    n++;
            }
        }
    }

    endTime = clock();
    printf("It took %f seconds to compute shortest path between cities with Floyd's algorithm as follows.\n", (float)endTime - startTime);

    return floyd_warshall_path;
}

int min(int a, int b)
{
    return (a > b) ? b : a;
}


void free_memory_forPath(Path *path)
{
    int nodeNum = path->graph->nodeNum;

    for (int i = 0; i < nodeNum; i++)
    {
        free(path->distance[i]);
        free(path->pi[i]);
    }
    free(path->distance);
    free(path->pi);
    //free(path);
}

void free_memory_forGraph(Graph *graph){
    int nodeNum = graph->nodeNum;

    for (int i = 0; i < nodeNum; i++)
    {
        free(graph->w[i]);
        free(graph->nodes[i]);
    }

    free(graph->w);
    free(graph->nodes);
    //free(graph);
}
