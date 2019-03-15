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
    char* transactionID;
    Wallet* senderWalletID;
    Wallet* receiverWalletID;
    unsigned long int value;
    time_t datetime;
} Transaction;

void printTransaction(Transaction* transaction);
void printTransactionList(LinkedList* transactionLinkedList);
void printBucket(Bucket* bucket, size_t bucketSize);
void printTransactionHashTable(HashTable* hashTable);

Transaction* initializeTransaction(char* transactionID, Wallet* senderWallet,
        Wallet* receiverWallet, unsigned long int value, char* date, char* _time);
int checkWalletIDInBucket(char* walletID, Bucket* bucket, size_t bucketSize,
    int walletType);
LinkedList* findWalletInTransactionHashTable(HashTable* hashTable, char* walletID, int walletType);
void insertToTransactionHashTable(HashTable* hashTable, Transaction* transaction,
    char* keyToHash, unsigned long int walletIDType);
int requestTransaction(Transaction* transaction, HashTable* walletHashTable,
    HashTable* senderHashTable, HashTable* receiverHashTable,
    unsigned long int bitcoinValue, time_t* latestTransactionTime);

void freeTransaction(Transaction* transaction);
void freeTransactionHashtable(HashTable* hashTable);
void freeTransactionHashtableCore(HashTable* hashTable);
#endif //ERGASIA_1_TRANSACTIONS_H