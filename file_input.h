#ifndef ERGASIA_1_FILE_INPUT_H
#define ERGASIA_1_FILE_INPUT_H
#include "transactions.h"

int readBitcoinBalancesFile(char* bitcoinBalancesFileName, HashTable** walletHashTable,
        HashTable** bitcoinHashTable, size_t bucketSize, unsigned long int bitcoinValue);
void readTransactionsFile(char* transactionFileName, HashTable* senderHashTable,
        HashTable* receiverHashTable, HashTable* walletHashTable,
        unsigned long int bitcoinValue, time_t* latestTransactionTime);
#endif //ERGASIA_1_FILE_INPUT_H