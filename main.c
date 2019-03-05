// Created by Antonis Karvelas.
// This is where it all begins...

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "verification.h"
#include "transactions.h"
#include "file_input.h"

int main(int argc, char **argv)
{
    // Initial command line parameters:
    int bitCoinValue = -1;
    char* bitCoinBalancesFile = NULL;
    char* transactionsFile = NULL;
    int senderHashtableNumOfEntries = -1;
    int receiverHashtableNumOfEntries = -1;
    int bucketSize = -1;

    // Read through the command line arguments:
    for(int i = 0; i < argc; i++)
    {
        // Get Bitcoin balances file:
        if(strcmp(argv[i], "-a") == 0)
        {
            i++;
            bitCoinBalancesFile = malloc(strlen(argv[i]) * sizeof(char));
            strcpy(bitCoinBalancesFile, argv[i]);
        }
        // Get transactions file:
        else if(strcmp(argv[i], "-t") == 0)
        {
            i++;
            transactionsFile = malloc(strlen(argv[i]) * sizeof(char));
            strcpy(transactionsFile, argv[i]);
        }
        // Get Bitcoin value:
        else if(strcmp(argv[i], "-v") == 0)
        {
            i++;
            bitCoinValue = atoi(argv[i]);
        }
        // Get number of entries for sender hash-table:
        else if(strcmp(argv[i], "-h1") == 0)
        {
            i++;
            senderHashtableNumOfEntries = atoi(argv[i]);
        }
        // Get number of entries for receiver hash-table:
        else if(strcmp(argv[i], "-h2") == 0)
        {
            i++;
            receiverHashtableNumOfEntries = atoi(argv[i]);
        }
        // Get bucket size:
        else if(strcmp(argv[i], "-b") == 0)
        {
            i++;
            bucketSize = atoi(argv[i]);
        }
    }

    // Check if all command line parameters are given properly:
    if(allParametersGiven(bitCoinValue, bitCoinBalancesFile, transactionsFile,
            senderHashtableNumOfEntries, receiverHashtableNumOfEntries, bucketSize))
    {
        printf("all good\n");
    }

    printf("\nasdfsdfssdlf\n");
    // Create sender hash table:
    printf("%i %i\n", senderHashtableNumOfEntries, bucketSize);
    HashTable* senderHashTable = initializeHashTable(senderHashtableNumOfEntries, bucketSize);
    HashTable* receiverHashTable = initializeHashTable(receiverHashtableNumOfEntries, bucketSize);

    printf("\nasdlf\n");
    // Read the transactions file and setup the hash tables:
    readTransactionsFile(transactionsFile, senderHashTable, receiverHashTable,
            senderHashtableNumOfEntries, receiverHashtableNumOfEntries);


    return 0;
}