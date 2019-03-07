#ifndef ERGASIA_1_FILE_WALLETS_H
#define ERGASIA_1_FILE_WALLETS_H
#include "transactions.h"
#include "hashtable.h"

typedef struct BitcoinNode
{
    int quantity;
    Transaction* transaction;
    struct BitcoinNode* child_a;
    struct BitcoinNode* child_b;
} BitcoinNode;

typedef struct BitcoinRoot
{
    int bitcoinID;
    BitcoinNode* rootNode;
} BitcoinRoot;

typedef struct Wallet
{
    char* walletID;
    LinkedList* bitcoins;
} Wallet;

BitcoinNode* initializeBitcoin(Transaction* transaction, int quantity);
BitcoinRoot* initializeBitcoinRoot(int bitcoinID, BitcoinNode* bitcoinNode);
int checkBitcoinInBucket(int bitcoinID, Bucket* bucket, size_t bucketSize);
int insertToBitcoinHashTable(HashTable* hashTable, BitcoinRoot* bitcoinRoot,
        char* keyToHash, int hashTableSize);

Wallet* initializeWallet(char* walletID);
int checkWalletInBucket(char* walletID, Bucket* bucket, size_t bucketSize);
int insertToWalletHashTable(HashTable* hashTable, Wallet* wallet,
        char* keyToHash, int hashTableSize);
#endif //ERGASIA_1_FILE_WALLETS_H