// Created by Antonis Karvelas.
// All data structures needed for this exercise are here.

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
    TransactionNode** transactions;
    struct Bucket* next;
} Bucket;


Bucket* initializeBucket(int bucketSize);
Bucket** initializeTransactionHashTable(int hashTableSize, int bucketSize);
void freeTransactionHashTable(Bucket** transactionHashTable, int hashTableSize);
int hash_function(char* string, int max);
