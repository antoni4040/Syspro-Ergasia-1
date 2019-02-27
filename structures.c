#include "structures.h"
#include <string.h>
#include <stdlib.h>

// Create a bucket and return it.
Bucket* initializeBucket(int bucketSize)
{
    Bucket* bucket = malloc(sizeof(Bucket));
    TransactionNode** bucketTransactions = malloc(bucketSize * sizeof(TransactionNode*));
    bucket->transactions = bucketTransactions;
    bucket->next = NULL;
    return bucket;
}

// Create a transaction hash table (an array of buckets basically) and return it.
Bucket** initializeTransactionHashTable(int hashTableSize, int bucketSize)
{
    Bucket** transactionHashTable = malloc(hashTableSize * sizeof(Bucket*));
    for(int i = 0; i < hashTableSize; i++)
    {
        transactionHashTable[i] = initializeBucket(bucketSize);
    }
    return transactionHashTable;
}

// Free the memory of a given transaction hash table.
void freeTransactionHashTable(Bucket** transactionHashTable, int hashTableSize)
{

}

// Hash a given string an return an index.
int hash_function(char* string, int max)
{
    int hash_value = 0;
    for(int i = 0; i < strlen(string); i++)
    {
        hash_value += string[i];
    }
    return hash_value % max;
}