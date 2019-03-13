#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>

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
}