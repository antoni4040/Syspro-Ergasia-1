#include "wallets.h"
#include <string.h>
#include <stdio.h>

void printWallet(Wallet* wallet)
{
    printf("Printing wallet: %s, balance %d\n", wallet->walletID, wallet->balance);
    Node* node = wallet->bitcoins->head;
    printf("Bitcoins: ");
    while(node != NULL)
    {
        printf("%i, ", ((BitcoinRoot*)(node->item))->bitcoinID);
        node = node->next;
    }
    printf("\n\n");
}

// Create a leaf bitcoin node:
BitcoinNode* initializeBitcoin(Transaction* transaction, Wallet* wallet, int quantity)
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
    newWallet->walletID = malloc((strlen(walletID) * sizeof(char))+1);
    strcpy(newWallet->walletID, walletID);
    newWallet->bitcoins = malloc(sizeof(LinkedList));
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
int insertToWalletHashTable(HashTable* hashTable, Wallet* wallet,
        char* keyToHash, int hashTableSize)
{
    int index = hash_function(keyToHash, hashTableSize);

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

    // If wallet is already in a bucket, then shit has hit the fun:
    else
    {
        return -1;
    }

}

// Return the wallet in the hashtable, NULL if it's not found:
Wallet* findWalletInHashTable(HashTable* hashTable, char* walletID)
{
    int hashTableSize = hashTable->size;
    int index = hash_function(walletID, hashTableSize);

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
int TreeBFSTransaction(BitcoinRoot* bitcoin, Transaction* transaction, int amount)
{
    int initialAmount = amount;
    Wallet* sender = transaction->senderWalletID;
    Wallet* receiver = transaction->receiverWalletID;
    Node* firstNode = initializeNode(bitcoin->rootNode);
    LinkedList* toVisit = initializeLinkedList(firstNode);
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
                    break;
                }
            }
        }
        else
        {
            // printf("%i %i\n", ((BitcoinNode*)(node->item))->child_a->quantity, ((BitcoinNode*)(node->item))->child_b->quantity);
            Node* nodeA = initializeNode(((BitcoinNode*)(node->item))->child_a);
            Node* nodeB = initializeNode(((BitcoinNode*)(node->item))->child_b);
            appendToLinkedList(toVisit, nodeA);
            appendToLinkedList(toVisit, nodeB);
        }
    }
    freeLinkedList(toVisit);
    return initialAmount - amount;
}