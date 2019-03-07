#include "transactions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printTransaction(Transaction* transaction, int bucket_index, int bucket)
{
    printf("%i %s %s %i bucket_index %i bucket %i\n", transaction->transactionID, transaction->senderWalletID,
            transaction->receiverWalletID, transaction->value, bucket_index, bucket);
}

void printTransactionList(LinkedList* transactionLinkedList,
        int bucket_index, int bucket)
{
    printf("TrList\n");
    Node* transactionNode = transactionLinkedList->head;
    do
    {
        printTransaction((Transaction*)(transactionNode->item), bucket_index, bucket);
        transactionNode = transactionNode->next;
    }while(transactionNode != NULL);
}

void printBucket(Bucket* bucket, int bucket_index, size_t bucketSize)
{
    for(int i = 0; i < bucketSize / sizeof(void*) - 2; i++)
    {
        if(bucket[i] != NULL)
            printTransactionList((LinkedList*)bucket[i], bucket_index, i);
    }
}

void printTransactionHashTable(HashTable* hashTable, int hashTableSize, size_t bucketSize)
{
    printf("tr\n");
    for(int i = 0; i < hashTableSize; i++)
    {
        Bucket* bucket = hashTable->buckets[i];
        printBucket(bucket, i, bucketSize);
        while(bucket[bucketSize / sizeof(void*) - 1] != NULL)
        {
            bucket = (Bucket*)bucket[bucketSize / sizeof(void*) - 1];
            printBucket(bucket, i, bucketSize);
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

// Return place of wallet id in bucket or -1 if it's not there:
int checkWalletIDInBucket(char* walletID, Bucket* bucket, size_t bucketSize, int walletType)
{
    int found = -1;
    for(int i = 0; i < bucketSize / sizeof(void*); i++)
    {
        if(bucket[i] == NULL)
            break;
        // Wallet type 1 for sender, 0 for receiver:
        if(walletType == 1)
        {
            if (strcmp(((Transaction*)(((LinkedList*)bucket[i])->head->item))->senderWalletID, walletID) == 0) {
                found = i;
                break;
            }
        }
        else
        {
            if (strcmp(((Transaction*)(((LinkedList*)bucket[i])->head->item))->receiverWalletID, walletID) == 0) {
                found = i;
                break;
            }
        }
    }
    return found;
}

// Add a new item in a hash table:
void insertToTransactionHashTable(HashTable* hashTable, Transaction* transaction,
        char* keyToHash, int hashTableSize, int walletIDType)
{
    // Package the transaction in a transaction node:
    Node* newTransactionNode = initializeNode(transaction);
    int index = hash_function(keyToHash, hashTableSize);

    // Go through buckets and see if a list exists:
    size_t bucketSize = hashTable->bucketSize;
    Bucket* bucketToInsert = hashTable->buckets[index];
    int position = -1;
    do
    {
        position = checkWalletIDInBucket(keyToHash, bucketToInsert, bucketSize, walletIDType);
        if(position != -1)
            break;
        bucketToInsert = (Bucket*)bucketToInsert[(bucketSize / sizeof(void*)) - 1];
    }while(bucketToInsert != NULL);

    // If wallet is not found, we have to create new list in a bucket:
    if(position == -1)
    {
        bucketToInsert = hashTable->buckets[index];
        while(bucketToInsert[(bucketSize / sizeof(void*)) - 1] != NULL)
            bucketToInsert = (Bucket*)bucketToInsert[(bucketSize / sizeof(void*)) - 1];
        LinkedList* newTransactionList = initializeLinkedList(newTransactionNode);
        insertToBucket(bucketToInsert, newTransactionList, bucketSize);
    }

    // If wallet is already in a bucket, then add transaction to the list:
    else
    {
        appendToLinkedList((LinkedList*)bucketToInsert[position], newTransactionNode);
    }

}