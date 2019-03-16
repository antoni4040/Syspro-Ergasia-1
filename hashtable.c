// Created by Antonis Karvelas.
// Here lies the generic hash table.
#include "hashtable.h"
#include <stdio.h>

// Create a bucket and return it. A bucket consists of generic pointers.
// The last void pointer points to the next bucket if it exists, otherwise it's
// NULL. The one before that is used as a counter. Even though it's a pointer,
// we treat it as a long int.
Bucket* initializeBucket(size_t bucketSize)
{
    Bucket* bucket = malloc(bucketSize);
    unsigned long int numOfEntries = bucketSize / sizeof(void*);
    for(unsigned long int i = 0; i < numOfEntries; i++)
    {
        bucket[i] = NULL;
    }
    bucket[numOfEntries - 2] = (void*)0;
    return bucket;
}

// Find the first empty place in a bucket and insert an item, or create a new
// bucket and insert it there:
void insertToBucket(Bucket* bucket, void* item, size_t bucketSize)
{
    unsigned long int numOfEntries = bucketSize / sizeof(void*);
    unsigned long int counter = (unsigned long int)bucket[numOfEntries - 2];

    // Check from counter if bucket is full:
    if(counter == numOfEntries - 2)
    {
        Bucket* newBucket = initializeBucket(bucketSize);
        bucket[numOfEntries - 1] = (void*)newBucket;
        insertToBucket((void*)bucket[numOfEntries - 1], item, bucketSize);
    }
    else
    {
        bucket[counter] = item;
        bucket[numOfEntries - 2] = (void*)(counter + 1);
    }
}

// Create a hash table (an array of buckets basically) and return it:
HashTable* initializeHashTable(unsigned long int hashTableSize, size_t bucketSize)
{
    HashTable* hashTable = malloc(sizeof(hashTable));
    hashTable->buckets = malloc(hashTableSize * sizeof(Bucket*));
    for(unsigned long int i = 0; i < hashTableSize; i++)
    {
        hashTable->buckets[i] = initializeBucket(bucketSize);
    }
    hashTable->size = hashTableSize;
    hashTable->bucketSize = bucketSize;
    return hashTable;
}

// Hash a given string an return an index:
unsigned long int hash_function(char* string, int max)
{
    unsigned long int hash_value = 0;
    for(int i = 0; i < strlen(string); i++)
    {
        hash_value += string[i];
    }
    return hash_value % max;
}