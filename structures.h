// Created by Antonis Karvelas.
// All data structures needed for this exercise are here.

#ifndef ERGASIA_1_STRUCTURES_H
#define ERGASIA_1_STRUCTURES_H

#include "hashtable.h"

typedef struct Date
{
    int day;
    int month;
    int year;
} Date;

typedef struct Time
{
    int hour;
    int minute;
}Time;

typedef struct Transaction
{
    int transactionID;
    char* senderWalletID;
    char* receiverWalletID;
    int value;
    Date* date;
    Time* time;
} Transaction;

typedef struct TransactionNode
{
    Transaction* transaction;
    struct TransactionNode* next;
} TransactionNode;

typedef struct TransactionLinkedList
{
    TransactionNode* head;
    TransactionNode* tail;
} TransactionLinkedList;

void printTransaction(Transaction* transaction, int bucket_index, int bucket);
void printBucket(Bucket* bucket, int bucket_index, size_t bucketSize);
void printTransactionHashTable(HashTable* hashTable, int hashTableSize, size_t bucketSize);
Time* initializeTime(char* timeString);
Date* initializeDate(char* dateString);
Transaction* initializeTransaction(int transactionID, char* senderWallet, char* receiverWallet,
                                   int value, char* date, char* time);
TransactionNode* initializeTransactionNode(Transaction* transaction);
TransactionLinkedList* initializeTransactionLinkedList(TransactionNode* transactionNode);
void insertToTransactionHashTable(HashTable* hashTable, Transaction* transaction,
                                  char* keyToHash, int hashTableSize, int walletIDType);
void freeTransactionHashTable(Bucket** transactionHashTable, int hashTableSize);
int hash_function(char* string, int max);

#endif //ERGASIA_1_STRUCTURES_H