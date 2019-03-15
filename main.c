// Created by Antonis Karvelas.
// This is where it all begins...
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "verification.h"
#include "transactions.h"
#include "file_input.h"
#include "commands.h"

int main(int argc, char **argv)
{
    // Initial command line parameters:
    unsigned long int bitCoinValue = 0;
    char* bitCoinBalancesFile = NULL;
    char* transactionsFile = NULL;
    unsigned long int senderHashtableNumOfEntries = 0;
    unsigned long int receiverHashtableNumOfEntries = 0;
    unsigned long int bucketSize = 0;

    time_t latestTransactionTime = 0;
    unsigned long int latestTransactionID = 0;

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
            bitCoinValue = strtoul(argv[i], NULL, 10);
        }
        // Get number of entries for sender hash-table:
        else if(strcmp(argv[i], "-h1") == 0)
        {
            i++;
            senderHashtableNumOfEntries = strtoul(argv[i], NULL, 10);
        }
        // Get number of entries for receiver hash-table:
        else if(strcmp(argv[i], "-h2") == 0)
        {
            i++;
            receiverHashtableNumOfEntries = strtoul(argv[i], NULL, 10);
        }
        // Get bucket size:
        else if(strcmp(argv[i], "-b") == 0)
        {
            i++;
            bucketSize = strtoul(argv[i], NULL, 10);
        }
    }

    // Check if all command line parameters are given properly:
    if(allParametersGiven(bitCoinValue, bitCoinBalancesFile, transactionsFile,
            senderHashtableNumOfEntries, receiverHashtableNumOfEntries, bucketSize))
    {
        printf("All parameters good. Begin transactions.\n");
    }

    // Create sender hash table:
    HashTable* senderHashTable = initializeHashTable(senderHashtableNumOfEntries, bucketSize);
    // Create receiver hash table:
    HashTable* receiverHashTable = initializeHashTable(receiverHashtableNumOfEntries, bucketSize);

    HashTable* bitcoinHashTable = NULL;
    HashTable* walletHashTable = NULL;

    // Read the wallet and bitcoin IDs:
    readBitcoinBalancesFile(bitCoinBalancesFile, &walletHashTable,
        &bitcoinHashTable, bucketSize, bitCoinValue);

    // Read the transactions file and setup the hash tables:
    readTransactionsFile(transactionsFile, senderHashTable, receiverHashTable, walletHashTable,
            bitCoinValue, &latestTransactionTime);

    commandLine(senderHashTable, receiverHashTable, walletHashTable);

    free(bitCoinBalancesFile);
    free(transactionsFile);
    freeTransactionHashtable(senderHashTable);
    freeTransactionHashtableCore(receiverHashTable);
    freeWalletHashTable(walletHashTable);
    freeBitcoinHashTable(bitcoinHashTable);
    return 0;
}