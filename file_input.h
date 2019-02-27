#ifndef ERGASIA_1_FILE_INPUT_H
#define ERGASIA_1_FILE_INPUT_H
#include "structures.h"

void readTransactionsFile(char* transactionFileName, Bucket** senderHashTable, Bucket** receiverHashTable, int senderHashTableSize, int receiverHashTableSize);
#endif //ERGASIA_1_FILE_INPUT_H