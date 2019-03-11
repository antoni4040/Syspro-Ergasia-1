#ifndef ERGASIA_1_FILE_WALLETS_H
#define ERGASIA_1_FILE_WALLETS_H
#include "transactions.h"
#include "hashtable.h"

typedef struct Transaction Transaction;

typedef struct Wallet
{
    char* walletID;
    int balance;
    LinkedList* bitcoins;
} Wallet;

typedef struct BitcoinNode
{
    int quantity;
    Transaction* transaction;
    Wallet* wallet;
    struct BitcoinNode* child_a;
    struct BitcoinNode* child_b;
} BitcoinNode;

typedef struct BitcoinRoot
{
    int bitcoinID;
    BitcoinNode* rootNode;
} BitcoinRoot;

BitcoinNode* initializeBitcoin(Transaction* transaction, Wallet* wallet, int quantity);
BitcoinRoot* initializeBitcoinRoot(int bitcoinID, BitcoinNode* bitcoinNode);
int checkBitcoinInBucket(int bitcoinID, Bucket* bucket, size_t bucketSize);
int insertToBitcoinHashTable(HashTable* hashTable, BitcoinRoot* bitcoinRoot,
        char* keyToHash, int hashTableSize);

Wallet* initializeWallet(char* walletID);
Wallet* findWalletInBucket(char* walletID, Bucket* bucket, size_t bucketSize);
int insertToWalletHashTable(HashTable* hashTable, Wallet* wallet,
        char* keyToHash, int hashTableSize);
Wallet* findWalletInHashTable(HashTable* hashTable, char* walletID);
#endif //ERGASIA_1_FILE_WALLETS_H