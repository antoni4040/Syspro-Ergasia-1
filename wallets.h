#ifndef ERGASIA_1_FILE_WALLETS_H
#define ERGASIA_1_FILE_WALLETS_H
#include "transactions.h"
#include "hashtable.h"

typedef struct Transaction Transaction;

typedef struct Wallet
{
    char* walletID;
    unsigned long int balance;
    LinkedList* bitcoins;
} Wallet;

typedef struct BitcoinNode
{
    unsigned long int quantity;
    Transaction* transaction;
    Wallet* wallet;
    struct BitcoinNode* child_a;
    struct BitcoinNode* child_b;
} BitcoinNode;

typedef struct BitcoinRoot
{
    unsigned long int bitcoinID;
    BitcoinNode* rootNode;
} BitcoinRoot;

void printWallet(Wallet* wallet);

BitcoinNode* initializeBitcoin(Transaction* transaction, Wallet* wallet,
    unsigned long int quantity);
BitcoinRoot* initializeBitcoinRoot(unsigned long int bitcoinID, BitcoinNode* bitcoinNode);
unsigned long int checkBitcoinInBucket(unsigned long int bitcoinID, Bucket* bucket, size_t bucketSize);
unsigned long int insertToBitcoinHashTable(HashTable* hashTable, BitcoinRoot* bitcoinRoot,
        char* keyToHash, unsigned long int hashTableSize);

Wallet* initializeWallet(char* walletID);
Wallet* findWalletInBucket(char* walletID, Bucket* bucket, size_t bucketSize);
unsigned long int insertToWalletHashTable(HashTable* hashTable, Wallet* wallet,
        char* keyToHash, unsigned long int hashTableSize);
Wallet* findWalletInHashTable(HashTable* hashTable, char* walletID);
unsigned long int TreeBFSTransaction(BitcoinRoot* bitcoin, Transaction* transaction,
    unsigned long int amount);
#endif //ERGASIA_1_FILE_WALLETS_H