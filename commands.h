#ifndef ERGASIA_1_COMMANDS_H
#define ERGASIA_1_COMMANDS_H

#include <stdio.h>
#include <string.h>
#include "transactions.h"

void commandLine(HashTable* senderHashTable, HashTable* receiverHashTable,
    HashTable* walletHashTable);
void requestTransactionCommand(char* line);
#endif //ERGASIA_1_COMMANDS_H