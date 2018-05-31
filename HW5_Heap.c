#include <stdio.h>
#include <stdlib.h>
#include "./HW5_Heap.h"

MinHeap Heap_init(){
    MinHeap hp;
    hp.size = 0;
    return hp;
}

void Heap_Insert(MinHeap *hp, int nodeIndex, int distance){
    //메모리 allocation
    if (hp->size !=0)
    { //이미 노드가 한개 이상 있으면
        Node *temp = realloc(hp->node, (hp->size + 1) * sizeof(Node));
        if(temp!=NULL)
            hp->node = temp;
    }
    else
    { //노드가 없으면
        hp->node = (Node*)malloc(sizeof(Node));
    }

    //집어넣을 Node를 하나 만들어서 값을 할당
    Node new;
    new.nodeIndex = nodeIndex;
    new.distance = distance;

    hp->size = hp->size + 1; //사이즈를 하나 늘려주고
    int i = hp->size -1; //마지막 자리에
    while (i && new.distance < hp->node[PARENT(i)].distance)
    { //부모보다 작으면, 부모로 올려주기
        hp->node[i] = hp->node[PARENT(i)];
        i = PARENT(i);
    }
    hp->node[i] = new;
}

int Heap_IsEmpty(MinHeap *hp){
    if (hp->size == 0){
         //free(hp->node);
         //free(hp);
         //================================================================================이부분에서 자꾸, free후에 메모리접근한다고 뜸
        return 1;
    }
        
    else
        return 0;
}
void swap(Node *n1, Node *n2)
{ //가리키고 있는 값이, 교환되고,
    Node temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}
int IsLeftNode(int index)
{
    if (index % 2 == 1)
        return 1;
    else
        return 0;
}
int IsRightNode(int index)
{
    if (index % 2 == 0)
        return 1;
    else
        return 0;
}
void PrintHeap(MinHeap *hp)
{
    for (int i = 0; i < hp->size; i++)
    {
        printf("%d -> ", hp->node[i].nodeIndex);
    }
    printf("\n");
}

//부모로 올라가면서 heapify하는 것임
void Heapify_Parent(MinHeap *hp, int i)
{
    //3. 부모로 올라갈 수 있는데까지 올라가기
    while (1)
    {
        if (i == 0 || hp->node[PARENT(i)].distance < hp->node[i].distance) //부모보다 크냐? 또는 루트노드냐??-> 그럼 종료해
        {
            if (i != 0 && IsLeftNode(i) && (hp->node[i].distance > hp->node[i + 1].distance))
            {
                swap(&hp->node[i], &hp->node[i + 1]); //형제끼리 바꾸고
                //printf("오른쪽 형제와 스왑\t");
                //PrintHeap(hp);
            }
            else if (i != 0 && IsRightNode(i) && (hp->node[i].distance < hp->node[i - 1].distance))
            {
                swap(&hp->node[i], &hp->node[i - 1]); //형제끼리 바꾸고
                //printf("왼쪽 형제와 스왑\t");
                //PrintHeap(hp);
            }

            break;
        }
        else // 부모노드와 교환
        {
            swap(&hp->node[PARENT(i)], &hp->node[i]);
            i = PARENT(i);
            //printf("부모와 스왑\t");
            //PrintHeap(hp);
        }
    }
}

//자식들만 보면서, heapify 하는 함수
void Heapify_Child(MinHeap *hp, int i)
{
    if (hp->node[LCHILD(i)].distance < hp->node[i].distance)
    { // 정상이 아님
        if (hp->node[RCHILD(i)].distance < hp->node[i].distance)
        {
            swap(&hp->node[RCHILD(i)], &hp->node[i]);
            Heapify_Child(hp, RCHILD(i));
            //R이랑 스왑
        }
        else if (hp->node[RCHILD(i)].distance > hp->node[i].distance)
        {
            swap(&hp->node[LCHILD(i)], &hp->node[i]);
            Heapify_Child(hp, LCHILD(i));
            //L이랑 스왑
        }
    }
}

void Heap_Update(MinHeap *hp, int name, int value){
    int i;

    while(i && hp->node[name].distance  )
    for (i = 0; i < hp->size; i++)
    {
        if (hp->node[i].nodeIndex == name)
        {
            hp->node[i].distance = value;
            //printf("힙의 %d index의 값을 %d 로 업데이트\n", i, value);
            Heapify_Parent(hp, i);
            break;
        }
    }
    
}

int Heap_remove(MinHeap *hp){ //아무것도 없으면 -1을 반환한다.
    int ret;
    Node *temp;
    if (hp->size)
    {
        ret = hp->node[0].nodeIndex; //최 상위에 있는 nodeIndex를 반환함
        hp->node[0] = hp->node[--(hp->size)];                      //맨 하위 노드가 부모자리로 올라온다.
        temp = realloc(hp->node, hp->size * sizeof(Node));
        if(temp != NULL){
            //hp->node = realloc(hp->node, hp->size * sizeof(Node)); //사이즈를 하나 줄인다.
            hp->node = temp;
        }
        
        Heapify_Child(hp, 0);                                  //부모노드의 제 위치를 찾아준다.
        return ret;
    }
    else{
        free(hp->node);
        return -1;
    }
}