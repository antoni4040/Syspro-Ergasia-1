// Created by Antonis Karvelas.

#include "hashtable.h"
#include <stdio.h>

// Create a bucket and return it:
Bucket* initializeBucket(size_t bucketSize)
{
    Bucket* bucket = malloc(bucketSize);
    for(int i = 0; i < bucketSize / sizeof(void*); i++)
    {
        bucket[i] = NULL;
    }
    return bucket;
}

// Check whether a bucket has a next bucket:
int checkBucketHasNext(Bucket* bucket, size_t bucketSize)
{
    if(bucket[(bucketSize / sizeof(void*)) - 1] == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// Find the first empty place in a bucket and insert an item, or create a new
// bucket and insert it there.
void insertToBucket(Bucket* bucket, void* item, size_t bucketSize)
{
    int currentIndex = 0;
    void* currentPointer = bucket[currentIndex];
    // Go through all the pointers in the bucket to find an empty place:
    while(currentPointer != NULL)
    {
        currentIndex++;
        currentPointer = bucket[currentIndex];
    }
    // If we've reached the last position, then we have to create a new bucket:
    if(currentIndex == (bucketSize / sizeof(void*)) - 1)
    {
        Bucket* newBucket = initializeBucket(bucketSize);
        bucket[currentIndex] = (void*)newBucket;
        insertToBucket((void*)bucket[currentIndex], item, bucketSize);
    }
    else
    {
        bucket[currentIndex] = item;
    }
}

// Create a hash table (an array of buckets basically) and return it:
HashTable* initializeHashTable(int hashTableSize, size_t bucketSize)
{
    HashTable* hashTable = malloc(sizeof(hashTable));
    hashTable->buckets = malloc(hashTableSize * sizeof(Bucket*));
    for(int i = 0; i < hashTableSize; i++)
    {
        hashTable->buckets[i] = initializeBucket(bucketSize);
    }
    printf("mallocdonesdf\n");
    hashTable->size = hashTableSize;
    hashTable->bucketSize = bucketSize;
    return hashTable;
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