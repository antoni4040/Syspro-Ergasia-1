#include "structures.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printTransaction(Transaction* transaction, int bucket_index, int bucket)
{
    printf("%i %s %s %i bucket_index %i bucket %i\n", transaction->transactionID, transaction->senderWalletID,
            transaction->receiverWalletID, transaction->value, bucket_index, bucket);
}

void printTransactionList(TransactionLinkedList* transactionLinkedList,int bucket_index, int bucket)
{
    TransactionNode* transactionNode = transactionLinkedList->head;
    do
    {
        printTransaction(transactionNode->transaction, bucket_index, bucket);
        transactionNode = transactionNode->next;
    }while(transactionNode != NULL);
}

void printBucket(Bucket* bucket, int bucket_index)
{
    for(int i = 0; i < bucket->currentIndex; i++)
    {
        printTransactionList(bucket->transactionLists[i], bucket_index, i);
    }
}

void printTransactionHashTable(Bucket** hashTable, int hashTableSize)
{
    for(int i = 0; i < hashTableSize; i++)
    {
        Bucket* bucket = hashTable[i];
        printBucket(bucket, i);
        while(bucket->next != NULL)
        {
            bucket = bucket->next;
            printBucket(bucket, i);
        }
    }
}

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

// Create a new transaction node with a given wallet ID and transaction:
TransactionNode* initializeTransactionNode(Transaction* transaction)
{
    TransactionNode* newTransactionNode = malloc(sizeof(TransactionNode));
    newTransactionNode->transaction = transaction;
    newTransactionNode->next = NULL;
    return newTransactionNode;
}

// Create a new transaction linked list and simply set the head and tail to the given transaction node:
TransactionLinkedList* initializeTransactionLinkedList(TransactionNode* transactionNode)
{
    TransactionLinkedList* newTransactionLinkedList = malloc(sizeof(TransactionLinkedList));
    newTransactionLinkedList->head = transactionNode;
    newTransactionLinkedList->tail = transactionNode;
    return newTransactionLinkedList;
}

// Add a transaction to a transaction linked list:
void addToTransactionLinkedList(TransactionLinkedList* transactionLinkedList, TransactionNode* transactionNode)
{
    transactionLinkedList->tail->next = transactionNode;
    transactionLinkedList->tail = transactionNode;
}

// Create a bucket and return it:
Bucket* initializeBucket(int bucketSize)
{
    Bucket* bucket = malloc(sizeof(Bucket));
    TransactionLinkedList** bucketTransactionLists = malloc(bucketSize * sizeof(TransactionLinkedList*));
    bucket->transactionLists = bucketTransactionLists;
    for(int i = 0; i < bucketSize; i++)
    {
        bucket->transactionLists[i] = NULL;
    }
    bucket->next = NULL;
    bucket->currentIndex = 0;
    bucket->size = bucketSize;
    return bucket;
}

// Return place of wallet id in bucket or -1 if it's not there:
int checkWalletIDInBucket(char* walletID, Bucket* bucket, int walletType)
{
    int found = -1;
    for(int i = 0; i < bucket->size; i++)
    {
        // Wallet type 1 for sender, 0 for receiver:
        if(walletType == 1)
        {
            if (strcmp(bucket->transactionLists[i]->head->transaction->senderWalletID, walletID) == 0) {
                found = i;
                break;
            }
        }
        else
        {
            if (strcmp(bucket->transactionLists[i]->head->transaction->receiverWalletID, walletID) == 0) {
                found = i;
                break;
            }
        }
    }
    return found;
}

// Add a transaction to a bucket or initialize another bucket and add it there if necessary:
void insertToBucket(Bucket* bucket, TransactionLinkedList* transactionLinkedList)
{
    if(bucket->currentIndex < bucket->size)
    {
        bucket->transactionLists[bucket->currentIndex] = transactionLinkedList;
        bucket->currentIndex++;
    }
    else
    {
        Bucket* newBucket = initializeBucket(bucket->size);
        bucket->next = newBucket;
        newBucket->transactionLists[0] = transactionLinkedList;
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
void insertToTransactionHashTable(Bucket** hashTable, Transaction* transaction,
        char* keyToHash, int hashTableSize, int walletIDType)
{
    // Package the transaction in a transaction node:
    TransactionNode* newTransactionNode = initializeTransactionNode(transaction);
    int index = hash_function(keyToHash, hashTableSize);

    // Go through buckets and see if a list exists:
    Bucket* bucketToInsert = hashTable[index];
    int position = -1;
    while(bucketToInsert->next != NULL)
    {
        position = checkWalletIDInBucket(keyToHash, bucketToInsert, walletIDType);
        if(position != -1)
            break;
        bucketToInsert = bucketToInsert->next;
    }

    // If wallet is not found, we have to create new list in a bucket:
    if(position == -1)
    {
        bucketToInsert = hashTable[index];
        while(bucketToInsert->next != NULL)
            bucketToInsert = bucketToInsert->next;
        TransactionLinkedList* newTransactionList = initializeTransactionLinkedList(newTransactionNode);
        insertToBucket(bucketToInsert, newTransactionList);
    }

    // If wallet is already in a bucket, then add transaction to the list:
    else
    {
        addToTransactionLinkedList(bucketToInsert->transactionLists[position], newTransactionNode);
    }

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