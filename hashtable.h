// Created by Antonis Karvelas.

#ifndef ERGASIA_1_HASHTABLE_H
#define ERGASIA_1_HASHTABLE_H
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef void** Bucket;

typedef struct HashTable
{
    Bucket** buckets;
    int size;
    size_t bucketSize;
} HashTable;

Bucket* initializeBucket(size_t bucketSize);
int checkBucketHasNext(Bucket* bucket, size_t bucketSize);
void insertToBucket(Bucket* bucket, void* item, size_t bucketSize);
HashTable* initializeHashTable(int hashTableSize, size_t bucketSize);
int hash_function(char* string, int max);

#endif //ERGASIA_1_HASHTABLE_H
