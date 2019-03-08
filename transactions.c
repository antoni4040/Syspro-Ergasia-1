#include "transactions.h"
#include "wallets.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printTransaction(Transaction* transaction, int bucket_index, int bucket)
{
    printf("%i %s %s %i %s bucket_index %i bucket %i\n", transaction->transactionID,
        transaction->senderWalletID, transaction->receiverWalletID, transaction->value,
        asctime(localtime(&transaction->datetime)),bucket_index, bucket);
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

// Create a transaction:
Transaction* initializeTransaction(int transactionID, char* senderWallet, char* receiverWallet,
        int value, char* date, char* _time)
{
    Transaction* newTransaction = malloc(sizeof(Transaction));
    newTransaction->transactionID = transactionID;
    newTransaction->senderWalletID = malloc(strlen(senderWallet) * sizeof(char));
    strcpy(newTransaction->senderWalletID, senderWallet);
    newTransaction->receiverWalletID = malloc(strlen(receiverWallet) * sizeof(char));
    strcpy(newTransaction->receiverWalletID, receiverWallet);
    newTransaction->value = value;

    // Convert date and time into timestamp. Much easier to handle:
    struct tm timestruct;
    char datetime[30];
    strcpy(datetime, date);
    strcat(datetime, _time);
    strptime(datetime, "%d-%m-%Y%H:%M", &timestruct);
    newTransaction->datetime = mktime(&timestruct);

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

// Check if the requested transaction is possible and do it. Engage.
int requestTransaction(Transaction* transaction, HashTable* walletHashTable,
    int walletHashTableSize, int bitcoinValue, time_t latestTransactionTime)
{
    char* sender = transaction->senderWalletID;
    char* receiver = transaction->receiverWalletID;
    int amount = transaction->value;

    // Check sender and receiver are different:
    if(strcmp(sender, receiver) == 0)
    {
        fprintf(stderr, "Sender and receiver can't be the same. What is this, money laundering?\n");
        return 1;
    }

    // Check transaction date:
    if(latestTransactionTime >= transaction->datetime)
    {
        fprintf(stderr, "Date-time error. Do you have a TARDIS or something?\n");
        return 1;
    }

    // Check sender has enough money:
    Wallet* senderWallet = findWalletInHashTable(walletHashTable, sender, walletHashTableSize);
    Wallet* receiverWallet = findWalletInHashTable(walletHashTable, receiver, walletHashTableSize);
    if(amount > senderWallet->balance)
    {
        fprintf(stderr, "Sender wallet doesn't have enough money. Just like me, actually...\n");
        return 1;
    }

    // If amount is larger that bitcoin value, take the first bitcoin from the
    // linked list in the sender and add it to the receiver:
    while(amount >= bitcoinValue)
    {
        Node* bitcoinNodeToTake = senderWallet->bitcoins->head;
        senderWallet->bitcoins->head = bitcoinNodeToTake->next;
        BitcoinNode* bitcoinToTake = ((BitcoinRoot*)(bitcoinNodeToTake->item))->rootNode;
        // Have to go through the God-damn tree:
    }
}