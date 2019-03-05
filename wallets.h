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

typedef struct Wallet
{
    char* walletID;
    HashTable* bitcoins;
} Wallet;

#endif //ERGASIA_1_FILE_WALLETS_H