#ifndef ERGASIA_1_COMMANDS_H
#define ERGASIA_1_COMMANDS_H
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include "transactions.h"
#include <time.h>
#include <stdlib.h>

void commandLine(HashTable* senderHashTable, HashTable* receiverHashTable,
    HashTable* walletHashTable, HashTable* bitcoinsHashTable, unsigned long int bitcoinValue,
    time_t* latestTransactionTime, unsigned long int* latestTransactionID);
void requestTransactionCommand(char* line, HashTable* wallets, HashTable* senders,
    HashTable* receivers, time_t* latestTransactionTime, unsigned long int* latestTransactionID,
    unsigned long int bitcoinValue);
void walletStatusCommand(char* line, HashTable* walletHashTable);
void findEarningsPaymentsCommand(HashTable* hashtable, char* line, int walletType);
void bitCoinStatusCommand(char* bitcoinID, HashTable* bitcoins, int doWhat);
#endif //ERGASIA_1_COMMANDS_H