#include <stdio.h>
#include <stdlib.h>
#include "./HW5_Heap.h"

MinHeap* Heap_init(MinHeap* hp){
    //MinHeap *hp;
    hp->size = 0;
    return hp;
}


void Heap_Insert_(MinHeap *hp, int index, int distance) {//distance만 있다고 생각해보자
    if(hp->size) {
        Node *temp = realloc(hp->node, (hp->size + 1) * sizeof(Node));
        if(temp != NULL)
            hp->node = temp;
    } else {
        hp->node = malloc(sizeof(Node)) ;
    }

    Node node ;
    node.distance = distance;
    node.nodeIndex = index;

    int i = hp->size;
    hp->size += 1;
    while(i && node.distance < hp->node[PARENT(i)].distance) {
        hp->node[i] = hp->node[PARENT(i)] ;
        i = PARENT(i) ;
    }
    hp->node[i] = node;
}

int Heap_IsEmpty(MinHeap *hp){
    if (hp->size == 0){
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
    if (index!=0 && index % 2 == 0)
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
void Heapify_Parent(MinHeap *hp, int i){

    int currentIndex = i;
    int heapsize = hp->size;
    while (1)
    {
        //PrintHeap(hp);
        if (currentIndex == 0 || hp->node[PARENT(currentIndex)].distance <= hp->node[currentIndex].distance) //부모보다 크냐? 또는 루트노드냐??-> 그럼 종료해
        {
            // //형제가 있는지부터 봐야함. 왼쪽노드인데, 오른쪽 형제가 없을수도 있잖아.
            // if (IsLeftNode(currentIndex) && currentIndex!=heapsize-1 && (hp->node[currentIndex].distance > hp->node[currentIndex + 1].distance))
            // {//왼쪽노드 && 마지막노드 아니고 && 오른쪽보다 크면 
            //     swap(&(hp->node[currentIndex]), &(hp->node[currentIndex + 1])); //형제끼리 바꾸고
            //     printf("오른쪽 형제와 스왑\t");
            //     PrintHeap(hp);
            // }
            // else if (IsRightNode(currentIndex) && (hp->node[currentIndex].distance < hp->node[currentIndex - 1].distance))
            // {
            //     swap(&(hp->node[currentIndex]), &(hp->node[currentIndex - 1])); //형제끼리 바꾸고
            //     printf("왼쪽 형제와 스왑\t");
            //     PrintHeap(hp);
            // }
            Heapify_Child(hp,currentIndex);

            break;
        }
        else// 부모노드와 교환
        {
            swap(&(hp->node[PARENT(currentIndex)]), &(hp->node[currentIndex]));
            currentIndex = PARENT(currentIndex);
            //printf("부모와 스왑\t");
            //PrintHeap(hp);
        }
    }
}

//자식들만 보면서, heapify 하는 함수
void Heapify_Child(MinHeap *hp, int i){   
    int leaf = hp->size -1;
    if(LCHILD(i)<leaf){

        if (hp->node[LCHILD(i)].distance < hp->node[i].distance)
        { // 정상이 아님
            if (hp->node[RCHILD(i)].distance < hp->node[i].distance)
            {
                swap(&(hp->node[RCHILD(i)]), &(hp->node[i]));
                Heapify_Child(hp, RCHILD(i));
                //R이랑 스왑
            }
            else
            {
                swap(&hp->node[LCHILD(i)], &hp->node[i]);
                Heapify_Child(hp, LCHILD(i));
                //L이랑 스왑
            }
        }
    }  
}

void Heap_Update(MinHeap *hp, int name, int value){

    int i;
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
    // if(i==hp->size){
    //     printf("해당 노드가 없어요\n");
    // }
}

int Heap_remove(MinHeap *hp){ //아무것도 없으면 -1을 반환한다.
    int removed;
    int leaf = hp->size -1;
    Node *temp;
    if (hp->size)
    {
        removed = hp->node[0].nodeIndex; //최 상위에 있는 nodeIndex를 반환함

        hp->node[0].distance = hp->node[leaf].distance;
        hp->node[0].nodeIndex = hp->node[leaf].nodeIndex;
        //printf("이제 최상위노드는 %d, 값은 %d\n",  hp->node[0].nodeIndex ,hp->node[0].distance);
        //PrintHeap(hp);
        hp->size -= 1;

        temp = realloc(hp->node, hp->size * sizeof(Node));
        if(temp != NULL){
            //hp->node = realloc(hp->node, hp->size * sizeof(Node)); //사이즈를 하나 줄인다.
            hp->node = temp;
        }
        
        Heapify_Child(hp, 0);                                  //부모노드의 제 위치를 찾아준다.
        return removed;
    }
    else{
        free(hp->node);
        return -1;
    }
}