// Created by Antonis Karvelas.
// Everything about wallets and bitcoins is in here.
#include "wallets.h"
#include <string.h>
#include <stdio.h>

void printWallet(Wallet* wallet)
{
    printf("Printing wallet: %s, balance %lu\n", wallet->walletID, wallet->balance);
    Node* node = wallet->bitcoins->head;
    printf("Bitcoins: ");
    while(node != NULL)
    {
        printf("%lu, ", ((BitcoinRoot*)(node->item))->bitcoinID);
        node = node->next;
    }
    printf("\n\n");
}

// Create a leaf bitcoin node:
BitcoinNode* initializeBitcoin(Transaction* transaction, Wallet* wallet,
    unsigned long int quantity)
{
    BitcoinNode* bitcoinNode = malloc(sizeof(BitcoinNode));
    bitcoinNode->quantity = quantity;
    bitcoinNode->transaction = transaction;
    bitcoinNode->wallet = wallet;
    bitcoinNode->child_a = NULL;
    bitcoinNode->child_b = NULL;
    return bitcoinNode;
}

// Create a root bitcoin. I just don't want to have the bitcoinID in multiple places...
BitcoinRoot* initializeBitcoinRoot(unsigned long int bitcoinID, BitcoinNode* bitcoinNode)
{
    BitcoinRoot* bitcoinRoot = malloc(sizeof(BitcoinRoot));
    bitcoinRoot->bitcoinID = bitcoinID;
    bitcoinRoot->rootNode = bitcoinNode;
    return bitcoinRoot;
}

// Return position if bitcoin found in bucket or -1 if it's not there:
unsigned long int checkBitcoinInBucket(unsigned long int bitcoinID,
    Bucket* bucket, size_t bucketSize)
{
    int found = -1;
    for(int i = 0; i < (bucketSize / sizeof(void*)) - 2; i++)
    {
        if(bucket[i] == NULL)
            break;
        else
        {
            if (((BitcoinRoot*)(bucket[i]))->bitcoinID == bitcoinID)
            {
                found = i;
                break;
            }
        }
    }
    return found;
}

// Return the bitcoin if found in the hashtable:
BitcoinRoot* findBitcoin(unsigned long int bitcoinID, HashTable* bitcoins)
{
    char bitcoinIDString[20];
    sprintf(bitcoinIDString, "%lu", bitcoinID);
    unsigned long int index = hash_function(bitcoinIDString, bitcoins->size);
    Bucket* bucket = bitcoins->buckets[index];
    int found = -1;
    do
    {
        found = checkBitcoinInBucket(bitcoinID, bucket, bitcoins->bucketSize);
        if(found != -1)
            break;
        bucket = (Bucket*)bucket[(bitcoins->bucketSize / sizeof(void*)) - 1];
    }while(bucket != NULL);

    if(found != -1)
    {
        return ((BitcoinRoot*)(bucket[found]));
    }
    else
        return NULL;
}

// Add a new bitcoin to the bitcoin hash table.
// Return 0 if this can be done, -1 if it's already in the hash table:
unsigned long int insertToBitcoinHashTable(HashTable* hashTable, BitcoinRoot* bitcoinRoot,
        char* keyToHash, unsigned long int hashTableSize)
{
    unsigned long int index = hash_function(keyToHash, hashTableSize);

    // Go through buckets and see if a list exists:
    size_t bucketSize = hashTable->bucketSize;
    Bucket* bucketToInsert = hashTable->buckets[index];
    int found = -1;
    do
    {
        found = checkBitcoinInBucket(atoi(keyToHash), bucketToInsert, bucketSize);
        if(found != -1)
            break;
        bucketToInsert = (Bucket*)bucketToInsert[(bucketSize / sizeof(void*)) - 1];
    }while(bucketToInsert != NULL);

    // If wallet is not found, we can add it to the hash table:
    if(found == -1)
    {
        bucketToInsert = hashTable->buckets[index];
        while(bucketToInsert[(bucketSize / sizeof(void*)) - 1] != NULL)
            bucketToInsert = (Bucket*)bucketToInsert[(bucketSize / sizeof(void*)) - 1];
        insertToBucket(bucketToInsert, bitcoinRoot, bucketSize);
        return 0;
    }

    // If wallet is already in a bucket, then shit has hit the fun:
    else
    {
        return -1;
    }

}

// Create a new wallet (why am I even bothering with these comments...):
Wallet* initializeWallet(char* walletID)
{
    Wallet* newWallet = malloc(sizeof(Wallet));
    newWallet->walletID = malloc((strlen(walletID) * sizeof(char))+1);
    strcpy(newWallet->walletID, walletID);
    newWallet->bitcoins = NULL;
    newWallet->balance = 0;
    return newWallet;
}

// Return wallet in bucket or NULL if it's not found:
Wallet* findWalletInBucket(char* walletID, Bucket* bucket, size_t bucketSize)
{
    Wallet* found = NULL;
    for(int i = 0; i < (bucketSize / sizeof(void*)) - 2; i++)
    {
        if(bucket[i] == NULL)
            break;
        else
        {
            if (strcmp(((Wallet*)(bucket[i]))->walletID, walletID) == 0)
            {
                found = (Wallet*)(bucket[i]);
                break;
            }
        }
    }
    return found;
}


// Add a new wallet to the wallet hash table.
// Return 0 if this can be done, -1 if it's already in the hash table:
unsigned long int insertToWalletHashTable(HashTable* hashTable, Wallet* wallet,
        char* keyToHash, unsigned long int hashTableSize)
{
    unsigned long int index = hash_function(keyToHash, hashTableSize);

    // Go through buckets and see if a list exists:
    size_t bucketSize = hashTable->bucketSize;
    Bucket* bucketToInsert = hashTable->buckets[index];
    Wallet* found = NULL;
    do
    {
        found = findWalletInBucket(keyToHash, bucketToInsert, bucketSize);
        if(found)
            break;
        bucketToInsert = (Bucket*)bucketToInsert[(bucketSize / sizeof(void*)) - 1];
    }while(bucketToInsert != NULL);

    // If wallet is not found, we can add it to the hash table:
    if(!found)
    {
        bucketToInsert = hashTable->buckets[index];
        while(bucketToInsert[(bucketSize / sizeof(void*)) - 1] != NULL)
            bucketToInsert = (Bucket*)bucketToInsert[(bucketSize / sizeof(void*)) - 1];
        insertToBucket(bucketToInsert, wallet, bucketSize);
        return 0;
    }

    // If wallet is already in a bucket, then shit has hit the fan:
    else
    {
        return -1;
    }

}

// Return the wallet in the hashtable, NULL if it's not found:
Wallet* findWalletInHashTable(HashTable* hashTable, char* walletID)
{
    unsigned long int hashTableSize = hashTable->size;
    unsigned long int index = hash_function(walletID, hashTableSize);

    // Go through buckets and see if a list exists:
    size_t bucketSize = hashTable->bucketSize;
    Bucket* bucket = hashTable->buckets[index];
    Wallet* wallet = NULL;
    do
    {
        wallet = findWalletInBucket(walletID, bucket, bucketSize);
        if(wallet)
            break;
        bucket = (Bucket*)bucket[(bucketSize / sizeof(void*)) - 1];
    }while(bucket != NULL);

    return wallet;
}

// Add the new transaction to the tree and return the amount that belongs to the sender:
unsigned long int TreeBFSTransaction(BitcoinRoot* bitcoin, Transaction* transaction,
    unsigned long int amount)
{
    unsigned long int initialAmount = amount;
    Wallet* sender = transaction->senderWalletID;
    Wallet* receiver = transaction->receiverWalletID;
    Node* firstNode = initializeNode(bitcoin->rootNode);
    LinkedList* toVisit = initializeLinkedList(firstNode);
    Node* nodeA;
    Node* nodeB;
    while(toVisit->head != NULL)
    {
        Node* node = popStart(toVisit);
        BitcoinNode* bitcoinNode = (BitcoinNode*)(node->item);
        // If bitcoin is a leaf node:
        // printf("%i %s\n", bitcoinNode->quantity, bitcoinNode->wallet->walletID);
        if(bitcoinNode->child_a == NULL)
        {
            if(bitcoinNode->wallet == sender)
            {
                if(bitcoinNode->quantity < amount)
                {
                    bitcoinNode->child_a = initializeBitcoin(transaction, receiver, bitcoinNode->quantity);
                    bitcoinNode->child_b = initializeBitcoin(transaction, sender, 0);
                    amount -= bitcoinNode->quantity;
                }
                else
                {
                    bitcoinNode->child_a = initializeBitcoin(transaction, receiver, amount);
                    bitcoinNode->child_b = initializeBitcoin(transaction,
                        sender, bitcoinNode->quantity - amount);
                    amount = 0;
                    free(node);
                    break;
                }
            }
        }
        else
        {
            // printf("%i %i\n", ((BitcoinNode*)(node->item))->child_a->quantity, ((BitcoinNode*)(node->item))->child_b->quantity);
            nodeA = initializeNode(((BitcoinNode*)(node->item))->child_a);
            nodeB = initializeNode(((BitcoinNode*)(node->item))->child_b);
            appendToLinkedList(toVisit, nodeA);
            appendToLinkedList(toVisit, nodeB);
        }
        free(node);
    }
    freeLinkedList(toVisit);
    return initialAmount - amount;
}

void freeWallet(Wallet* wallet)
{
    free(wallet->walletID);
    freeLinkedList(wallet->bitcoins);
    free(wallet);
}

void freeBitcoinNode(BitcoinNode* bitcoinNode)
{
    if(bitcoinNode == NULL)
        return;
    else
    {
        freeBitcoinNode(bitcoinNode->child_a);
        freeBitcoinNode(bitcoinNode->child_b);
    }
    free(bitcoinNode);
}

void freeBitcoinTree(BitcoinRoot* bitcoinRoot)
{
    freeBitcoinNode(bitcoinRoot->rootNode);
    free(bitcoinRoot);
}

void freeWalletHashTable(HashTable* wallets)
{
    for(unsigned long int i = 0; i < wallets->size; i++)
    {
        Bucket* bucket = wallets->buckets[i];
        while(bucket != NULL)
        {
            for(unsigned long int j = 0; j < ((wallets->bucketSize) / sizeof(void*) - 2); j++)
            {
                if(bucket[j] == NULL)
                    break;
                Wallet* wallet = (Wallet*)(bucket[j]);
                freeWallet(wallet);
            }
            Bucket* currentBucket = bucket;
            bucket = (Bucket*)(bucket[(wallets->bucketSize) / sizeof(void*) - 1]);
            free(currentBucket);
        }
    }
    free(wallets->buckets);
    free(wallets);
}

void freeBitcoinHashTable(HashTable* bitcoins)
{
    for(unsigned long int i = 0; i < bitcoins->size; i++)
    {
        Bucket* bucket = bitcoins->buckets[i];
        while(bucket != NULL)
        {
            for(unsigned long int j = 0; j < ((bitcoins->bucketSize) / sizeof(void*) - 2); j++)
            {
                if(bucket[j] == NULL)
                    break;
                BitcoinRoot* bitcoinRoot = (BitcoinRoot*)(bucket[j]);
                freeBitcoinTree(bitcoinRoot);
            }
            Bucket* currentBucket = bucket;
            bucket = (Bucket*)(bucket[(bitcoins->bucketSize) / sizeof(void*) - 1]);
            free(currentBucket);
        }
    }
    free(bitcoins->buckets);
    free(bitcoins);
}