#include "wallets.h"
#include <string.h>

// Create a leaf bitcoin node:
BitcoinNode* initializeBitcoin(Transaction* transaction, int quantity)
{
    BitcoinNode* bitcoinNode = malloc(sizeof(BitcoinNode));
    bitcoinNode->quantity = quantity;
    bitcoinNode->transaction = transaction;
    bitcoinNode->child_a = NULL;
    bitcoinNode->child_b = NULL;
    return bitcoinNode;
}

// Create a root bitcoin. I just don't want to have the bitcoinID in multiple places...
BitcoinRoot* initializeBitcoinRoot(int bitcoinID, BitcoinNode* bitcoinNode)
{
    BitcoinRoot* bitcoinRoot = malloc(sizeof(BitcoinRoot));
    bitcoinRoot->bitcoinID = bitcoinID;
    bitcoinRoot->rootNode = bitcoinNode;
    return bitcoinRoot;
}

// Return 1 if bitcoin found in bucket or -1 if it's not there:
int checkBitcoinInBucket(int bitcoinID, Bucket* bucket, size_t bucketSize)
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
                found = 1;
                break;
            }
        }
    }
    return found;
}

// Add a new bitcoin to the bitcoin hash table.
// Return 0 if this can be done, -1 if it's already in the hash table:
int insertToBitcoinHashTable(HashTable* hashTable, BitcoinRoot* bitcoinRoot,
        char* keyToHash, int hashTableSize)
{
    int index = hash_function(keyToHash, hashTableSize);

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
    newWallet->walletID = malloc(strlen(walletID) * sizeof(char));
    strcpy(newWallet->walletID, walletID);
    newWallet->bitcoins = malloc(sizeof(LinkedList));
    return newWallet;
}

// Return 1 if wallet found in bucket or -1 if it's not there:
int checkWalletInBucket(char* walletID, Bucket* bucket, size_t bucketSize)
{
    int found = -1;
    for(int i = 0; i < (bucketSize / sizeof(void*)) - 2; i++)
    {
        if(bucket[i] == NULL)
            break;
        else
        {
            if (strcmp(((Wallet*)(bucket[i]))->walletID, walletID) == 0)
            {
                found = 1;
                break;
            }
        }
    }
    return found;
}

// Add a new wallet to the wallet hash table.
// Return 0 if this can be done, -1 if it's already in the hash table:
int insertToWalletHashTable(HashTable* hashTable, Wallet* wallet,
        char* keyToHash, int hashTableSize)
{
    int index = hash_function(keyToHash, hashTableSize);

    // Go through buckets and see if a list exists:
    size_t bucketSize = hashTable->bucketSize;
    Bucket* bucketToInsert = hashTable->buckets[index];
    int found = -1;
    do
    {
        found = checkWalletInBucket(keyToHash, bucketToInsert, bucketSize);
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
        insertToBucket(bucketToInsert, wallet, bucketSize);
        return 0;
    }

    // If wallet is already in a bucket, then shit has hit the fun:
    else
    {
        return -1;
    }

}