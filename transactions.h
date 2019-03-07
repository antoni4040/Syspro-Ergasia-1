// Created by Antonis Karvelas.
// Everything that has to do with transactions in here.

#ifndef ERGASIA_1_TRANSACTIONS_H
#define ERGASIA_1_TRANSACTIONS_H

#include "hashtable.h"
#include "linked_list.h"

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

void printTransaction(Transaction* transaction, int bucket_index, int bucket);
void printTransactionList(LinkedList* transactionLinkedList,
        int bucket_index, int bucket);
void printBucket(Bucket* bucket, int bucket_index, size_t bucketSize);
void printTransactionHashTable(HashTable* hashTable, int hashTableSize, size_t bucketSize);

Time* initializeTime(char* timeString);
Date* initializeDate(char* dateString);

Transaction* initializeTransaction(int transactionID, char* senderWallet, char* receiverWallet,
                                   int value, char* date, char* time);
void insertToTransactionHashTable(HashTable* hashTable, Transaction* transaction,
                                  char* keyToHash, int hashTableSize, int walletIDType);

#endif //ERGASIA_1_TRANSACTIONS_H