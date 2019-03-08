// Created by Antonis Karvelas.
// Here lies the generic linked list.
// Quite a lot of generic data structures, but I feel shame when I have to
// rewrite code that I could otherwise abstract a bit.

#ifndef ERGASIA_1_LINKEDLIST_H
#define ERGASIA_1_LINKEDLIST_H

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

Node* initializeNode(void* item);
LinkedList* initializeLinkedList(Node* node);
void appendToLinkedList(LinkedList* linkedList, Node* node);
void prependToLinkedList(LinkedList* linkedList, Node* node);
Node* popStart(LinkedList* linkedList);
#endif //ERGASIA_1_LINKEDLIST_H