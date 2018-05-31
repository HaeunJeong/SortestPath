#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "HW5_Graph.h"

char *trim(char *s)
{
    int count = strlen(s);
    char *new;

    while (isspace(s[count - 1]))
    {
        count--;
        if (count == 0)
            break;
    }
    new = (char *)malloc(sizeof(char) * count);
    strncpy(new, s, count);
    new[count] = '\0';

    return new;
}

Graph *init_Graph(FILE *fp)
{

    Graph *graph = (Graph *)malloc(sizeof(Graph *)); //그래프 메모리 동적 할당
    //Edge edges[100];//==============================================================
    //graph->edges = (Edge*)malloc(sizeof(Edge));

    //Edge *edges = malloc(sizeof(Edge));

    char buffer[255], copy[255]; //read one line
    char node[10];
    int nodeNum = 0; // 총 노드 갯수
    int i = 0;
    int edge_count = 0;

    fgets(buffer, sizeof(buffer), fp);
    strcpy(copy, buffer);

    char *ptr = strtok(buffer, "\t "); //탭과 스페이스를 기준으로 자른다.
    while (ptr != NULL)
    {
        nodeNum++;
        ptr = strtok(NULL, "\t ");
    } //node의 갯수 구하기
    graph->nodeNum = nodeNum;

    //char **Node = (char**)malloc(sizeof(node)*nodeNum);
    graph->nodes = (char **)malloc(sizeof(node) * nodeNum);

    ptr = strtok(copy, "\t "); //탭과 스페이스를 기준으로 자른다.

    for (int i = 0; i < nodeNum; i++)
    {
        graph->nodes[i] = trim(ptr);
        ptr = strtok(NULL, "\t ");
    }

    //=============== Node배열에, node들 할당 완료 ===================//===============================이부분 다시 수정해야함

    graph->w = (int **)malloc(sizeof(int *) * nodeNum); //매트릭스 메모리 할당
    for (i = 0; i < nodeNum; i++)
    {
        graph->w[i] = (int *)malloc(sizeof(int) * nodeNum); //매크릭스 [i-row] 메모리 할당 얘 크기 바꾸자
        memset(graph->w[i], 0, sizeof(int) * nodeNum);      //메모리 0으로 초기화
    }

    //============ 그래프의 기본틀 만들기 완료 ======================//

    for (int i = 0; i < nodeNum; i++)
    { //node갯수만큼의 줄을 읽는다.
        buffer[0] = '\0';
        fgets(buffer, sizeof(buffer), fp);

        ptr = strtok(buffer, "\t ");

        for (int j = 0; j < nodeNum; j++)
        {
            ptr = strtok(NULL, "\t "); //첫 도시 이름은 스킵해야하므로
            ptr = trim(ptr);
        
            if (strcmp(ptr, "INF")){ //INF가 아니면
                graph->w[i][j] = atoi(ptr);
            
                //edge값 추가
                if (strcmp(ptr, "0") && j > i){ //겹치지 않은 엣지만 찾아낸다.
                    edge_count++;
                    //printf("edge num: %d  ", edge_count);
                    //edges = (Edge*)realloc(edges,sizeof(Edge)*edge_count);

                    graph->edges[edge_count - 1].node_a = i;
                    graph->edges[edge_count - 1].node_b = j;
                    //edges[edge_count-1].node_a = i;
                    //edges[edge_count-1].node_b = j;
                }
            }else{
                graph->w[i][j] = NO; //연결되어있는 노드가 없으면 NO(-1)값을 넣어준다.

            }
            //printf("w[%d][%d] = %d\n", i,j,graph->w[i][j]);
        }
    }

    //Edge *edges = (Edge*)malloc(sizeof(Edge)*edge_count);

    //graph->edges = edges;
    graph->edgeNum = edge_count;

    return graph;
}
                    // graph->edges = (Edge*)realloc(graph->edges, edge_count * sizeof(Edge));

                    // Edge new;
                    // new.node_a = i;
                    // new.node_a = j;

                    // graph->edges[edge_count-1] = new;


