#ifndef ERGASIA_1_COMMANDS_H
#define ERGASIA_1_COMMANDS_H
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include "transactions.h"
#include <time.h>

void commandLine(HashTable* senderHashTable, HashTable* receiverHashTable,
    HashTable* walletHashTable);
void requestTransactionCommand(char* line);
void walletStatusCommand(char* line, HashTable* walletHashTable);
void findEarningsPaymentsCommand(HashTable* hashtable, char* line, int walletType);
#endif //ERGASIA_1_COMMANDS_H