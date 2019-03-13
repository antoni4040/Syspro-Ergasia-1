#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct Node
{
    void* item;
    struct Node* next;
} Node;

typedef struct LinkedList
{
    Node* head;
    Node* tail;
} LinkedList;

// Create a new linked list node with a given item:
Node* initializeNode(void* item)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->item = item;
    newNode->next = NULL;
    return newNode;
}

// Create a new linked list and simply set the head and tail to the given node:
LinkedList* initializeLinkedList(Node* node)
{
    LinkedList* newLinkedList = malloc(sizeof(LinkedList));
    newLinkedList->head = node;
    newLinkedList->tail = node;
    return newLinkedList;
}

// Append item to a linked list:
void appendToLinkedList(LinkedList* linkedList, Node* node)
{
    if(linkedList->head == NULL)
    {
        linkedList->head = node;
        linkedList->tail = node;
    }
    else
    {
        linkedList->tail->next = node;
        linkedList->tail = node;
    }
}

// Pop a node from the beginning of the list:
Node* popStart(LinkedList* linkedList)
{
    Node* node = linkedList->head;
    linkedList->head = linkedList->head->next;
    return node;
}

// Deallocate memory of a linked list:
void freeLinkedList(LinkedList* linkedList)
{
    Node* node;
    while((node = linkedList->head) != NULL)
    {
        linkedList->head = linkedList->head->next;
        free(node);
    }
    free(linkedList);
}

struct a
{
    int num;
};

int main()
{
    void** a = malloc((size_t)(64));
    int b = 4;
    for(int i = 0; i < 64 / sizeof(void*); i++)
    {
        a[i] = &b;
    }

    a[6] = (int*)10;

    for(int i = 0; i < 64 / sizeof(void*); i++)
    {
        printf("%li\n", (long int)(int*)a[i]);
    }

    struct a* s = malloc(sizeof(a));
    s->num = 5;

    struct a* z = malloc(sizeof(a));
    z->num = 4;

    struct a* n = malloc(sizeof(a));
    n->num = 3;

    Node* node = initializeNode(s);
    LinkedList* list = initializeLinkedList(node);

    node = initializeNode(z);
    appendToLinkedList(list, node);

    node = initializeNode(n);
    appendToLinkedList(list, node);

    Node* nodePop;

    while(list->head)
    {
        nodePop = popStart(list);
        printf("%i\n", ((struct a*)(nodePop->item))->num);
    }
    return 0;
}