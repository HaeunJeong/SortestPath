#ifndef HW5_HEAP_H
#define HW5_HEAP_H

#define LCHILD(x) 2 * x + 1
#define RCHILD(x) 2 * x + 2
#define PARENT(x) (x - 1) / 2

typedef struct
{
    int nodeIndex;
    int distance;
} Node;

typedef struct
{
    int size;
    Node *node;
} MinHeap;

//MinHeap Heap_init();
MinHeap *Heap_init();
void Heap_Insert(MinHeap *hp, int nodeIndex, int distance);
int Heap_IsEmpty(MinHeap *hp);
void swap(Node *n1, Node *n2);
int IsLeftNode(int index);
int IsRightNode(int index);
void PrintHeap(MinHeap *hp);
void Heapify_Parent(MinHeap *hp, int i);
void Heapify_Child(MinHeap *hp, int i);
void Heap_Update(MinHeap *hp, int name, int value);
int Heap_remove(MinHeap *hp);

#endif