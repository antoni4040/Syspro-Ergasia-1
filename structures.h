// Created by Antonis Karvelas.
// All data structures needed for this exercise are here.

#ifndef ERGASIA_1_STRUCTURES_H
#define ERGASIA_1_STRUCTURES_H

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
    char* walletID;
    Transaction* transaction;
    struct TransactionNode* next;
} TransactionNode;

typedef struct Bucket
{
    Transaction** transactions;
    int currentIndex;
    int size;
    struct Bucket* next;
} Bucket;

Time* initializeTime(char* timeString);
Date* initializeDate(char* dateString);
Transaction* initializeTransaction(int transactionID, char* senderWallet, char* receiverWallet,
                                   int value, char* date, char* time);
Bucket* initializeBucket(int bucketSize);
void insertToBucket(Bucket* bucket, Transaction* transaction);
Bucket** initializeTransactionHashTable(int hashTableSize, int bucketSize);
void insertToTransactionHashTable(Bucket** hashTable, Transaction* transaction, char* keyToHash, int hashTableSize);
void freeTransactionHashTable(Bucket** transactionHashTable, int hashTableSize);
int hash_function(char* string, int max);

#endif //ERGASIA_1_STRUCTURES_H