#include "linked_list.h"
#include <stdio.h>

// Create a new linked list node with a given item:
Node* initializeTransactionNode(void* item)
{
    Node* newNode = malloc(sizeof(Node));
    newNode->item = item;
    newNode->next = NULL;
    return newNode;
}

// Create a new linked list and simply set the head and tail to the given node:
LinkedList* initializeTransactionLinkedList(Node* node)
{
    LinkedList* newLinkedList = malloc(sizeof(LinkedList));
    newLinkedList->head = node;
    newLinkedList->tail = node;
    return newLinkedList;
}

// Append item to a linked list:
void appendToLinkedList(LinkedList* linkedList, Node* node)
{
    linkedList->tail->next = node;
    linkedList->tail = node;
}