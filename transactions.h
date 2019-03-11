// Created by Antonis Karvelas.
// Everything that has to do with transactions in here.

#ifndef ERGASIA_1_TRANSACTIONS_H
#define ERGASIA_1_TRANSACTIONS_H
#define _XOPEN_SOURCE 700
#include "hashtable.h"
#include "linked_list.h"
#include "wallets.h"
#include <time.h>

typedef struct Wallet Wallet;

typedef struct Transaction
{
    int transactionID;
    Wallet* senderWalletID;
    Wallet* receiverWalletID;
    int value;
    time_t datetime;
} Transaction;

void printTransaction(Transaction* transaction, int bucket_index, int bucket);
void printTransactionList(LinkedList* transactionLinkedList,
        int bucket_index, int bucket);
void printBucket(Bucket* bucket, int bucket_index, size_t bucketSize);
void printTransactionHashTable(HashTable* hashTable, int hashTableSize, size_t bucketSize);

Transaction* initializeTransaction(int transactionID, Wallet* senderWallet,
        Wallet* receiverWallet, int value, char* date, char* _time);
void insertToTransactionHashTable(HashTable* hashTable, Transaction* transaction,
                                  char* keyToHash, int hashTableSize, int walletIDType);
int requestTransaction(Transaction* transaction, HashTable* walletHashTable,
    int walletHashTableSize, int bitcoinValue,time_t latestTransactionTime);

#endif //ERGASIA_1_TRANSACTIONS_H