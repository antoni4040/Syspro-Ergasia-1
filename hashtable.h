// Created by Antonis Karvelas.
// Here lies the generic hash table.

#ifndef ERGASIA_1_HASHTABLE_H
#define ERGASIA_1_HASHTABLE_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef void** Bucket;

typedef struct HashTable
{
    Bucket** buckets;
    unsigned long int size;
    size_t bucketSize;
} HashTable;

Bucket* initializeBucket(size_t bucketSize);
int checkBucketHasNext(Bucket* bucket, size_t bucketSize);
void insertToBucket(Bucket* bucket, void* item, size_t bucketSize);
HashTable* initializeHashTable(unsigned long int hashTableSize, size_t bucketSize);
unsigned long int hash_function(char* string, int max);

#endif //ERGASIA_1_HASHTABLE_H
