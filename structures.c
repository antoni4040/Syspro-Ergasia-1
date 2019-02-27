#include "structures.h"
#include <string.h>
#include <stdlib.h>

// Create a new time struct from a string:
Time* initializeTime(char* timeString)
{
    Time* newTime = malloc(sizeof(Time));
    newTime->hour = atoi(strtok(timeString, ":"));
    newTime->minute = atoi(strtok(NULL, "-"));
    return newTime;
}

// Create a new date struct from a string:
Date* initializeDate(char* dateString)
{
    Date* newDate = malloc(sizeof(Date));
    newDate->day =  atoi(strtok(dateString, "-"));
    newDate->month =  atoi(strtok(NULL, "-"));
    newDate->year =  atoi(strtok(NULL, " "));
    return newDate;
}

// Create a transaction:
Transaction* initializeTransaction(int transactionID, char* senderWallet, char* receiverWallet,
        int value, char* date, char* time)
{
    Transaction* newTransaction = malloc(sizeof(Transaction));
    newTransaction->transactionID = transactionID;
    newTransaction->senderWalletID = malloc(strlen(senderWallet) * sizeof(char));
    strcpy(newTransaction->senderWalletID, senderWallet);
    newTransaction->receiverWalletID = malloc(strlen(receiverWallet) * sizeof(char));
    strcpy(newTransaction->receiverWalletID, receiverWallet);
    newTransaction->value = value;
    newTransaction->date = initializeDate(date);
    newTransaction->time = initializeTime(time);
    return newTransaction;
}

// Create a bucket and return it:
Bucket* initializeBucket(int bucketSize)
{
    Bucket* bucket = malloc(sizeof(Bucket));
    Transaction** bucketTransactions = malloc(bucketSize * sizeof(Transaction*));
    bucket->transactions = bucketTransactions;
    for(int i = 0; i < bucketSize; i++)
    {
        bucket->transactions[i] = NULL;
    }
    bucket->next = NULL;
    bucket->currentIndex = 0;
    bucket->size = bucketSize;
    return bucket;
}

// Add a transaction to a bucket or initialize another bucket and add it there if necessary:
void insertToBucket(Bucket* bucket, Transaction* transaction)
{
    if(bucket->currentIndex < bucket->size)
    {
        bucket->transactions[bucket->currentIndex] = transaction;
        bucket->currentIndex++;
    }
    else
    {
        Bucket* newBucket = initializeBucket(bucket->size);
        bucket->next = newBucket;
        newBucket->transactions[0] = transaction;
        newBucket->currentIndex++;
    }
}

// Create a transaction hash table (an array of buckets basically) and return it:
Bucket** initializeTransactionHashTable(int hashTableSize, int bucketSize)
{
    Bucket** transactionHashTable = malloc(hashTableSize * sizeof(Bucket*));
    for(int i = 0; i < hashTableSize; i++)
    {
        transactionHashTable[i] = initializeBucket(bucketSize);
    }
    return transactionHashTable;
}

// Add a new item in a hash table:
void insertToTransactionHashTable(Bucket** hashTable, Transaction* transaction, char* keyToHash, int hashTableSize)
{
    int index = hash_function(keyToHash, hashTableSize);
    // Find the latest bucket:
    Bucket* bucketToInsert = hashTable[index];
    while(bucketToInsert->next != NULL)
        bucketToInsert = bucketToInsert->next;

}

// Free the memory of a given transaction hash table:
void freeTransactionHashTable(Bucket** transactionHashTable, int hashTableSize)
{

}

// Hash a given string an return an index:
int hash_function(char* string, int max)
{
    int hash_value = 0;
    for(int i = 0; i < strlen(string); i++)
    {
        hash_value += string[i];
    }
    return hash_value % max;
}