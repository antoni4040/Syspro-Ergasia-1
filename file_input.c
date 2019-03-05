// Created by Antonis Karvelas.
// Everything about opening and reading data from files resides here.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_input.h"
#include "transactions.h"

#define BUFFER_SIZE 256

void readTransactionsFile(char* transactionFileName, HashTable* senderHashTable, HashTable* receiverHashTable,
        int senderHashTableSize, int receiverHashTableSize)
{
    FILE* transactionFile = fopen(transactionFileName, "r");

    int transactionID;
    char* senderWallet;
    char* receiverWallet;
    int value;
    char* date;
    char* time;

    printf("\n alsdkfja\n");

    //Check transaction file name is correct:
    if(transactionFile == NULL)
    {
        fprintf(stderr, "No transaction file found.\n");
    }

    //Start reading the file line by line:
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), transactionFile)) {
        transactionID = atoi(strtok(buffer, " "));
        senderWallet = strtok(NULL, " ");
        receiverWallet = strtok(NULL, " ");
        value = atoi(strtok(NULL, " "));
        date = strtok(NULL, " ");
        time = strtok(NULL, " ");
        Transaction* newTransaction = initializeTransaction(transactionID, senderWallet, receiverWallet, value, date, time);
        insertToTransactionHashTable(senderHashTable, newTransaction, senderWallet, senderHashTableSize, 1);
        insertToTransactionHashTable(receiverHashTable, newTransaction, receiverWallet, receiverHashTableSize, 0);
    }

    printTransactionHashTable(senderHashTable, senderHashTableSize, senderHashTable->bucketSize);
}