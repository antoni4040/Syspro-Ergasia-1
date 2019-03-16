#ifndef ERGASIA_1_FILE_INPUT_H
#define ERGASIA_1_FILE_INPUT_H
#include "transactions.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int readBitcoinBalancesFile(char* bitcoinBalancesFileName, HashTable** walletHashTable,
        HashTable** bitcoinHashTable, size_t bucketSize, unsigned long int bitcoinValue);
void readTransactionsFile(char* transactionFileName, HashTable* senderHashTable,
        HashTable* receiverHashTable, HashTable* walletHashTable,
        unsigned long int bitcoinValue, time_t* latestTransactionTime,
        unsigned long int* latestTransactionID);
#endif //ERGASIA_1_FILE_INPUT_H