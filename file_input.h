#ifndef ERGASIA_1_FILE_INPUT_H
#define ERGASIA_1_FILE_INPUT_H
#include "transactions.h"

void readTransactionsFile(
    char* transactionFileName, HashTable* senderHashTable, HashTable* receiverHashTable,
                        int senderHashTableSize, int receiverHashTableSize);
#endif //ERGASIA_1_FILE_INPUT_H