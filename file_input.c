// Created by Antonis Karvelas.
// Everything about opening and reading data from files resides here.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_input.h"
#include "transactions.h"
#include "wallets.h"

#define BUFFER_SIZE 256

int readBitcoinBalancesFile(char* bitcoinBalancesFileName, HashTable* walletHashTable,
        HashTable* bitcoinHashTable, size_t bucketSize, int bitcoinValue)
{
    FILE* bitcoinBalancesFile = fopen(bitcoinBalancesFileName, "r");

    // We want to count the number of wallets and bitcoins first:
    long int numberOfWallets = 0;
    long int numberOfBitcoins = 0;
    long int walletHashTableNumOfEntries;
    long int bitcoinHashTableNumOfEntries;

    char* walletID;
    int bitcoinID;
    int inserted;

    //Check bitcoin balances file name is correct:
    if(bitcoinBalancesFile == NULL)
    {
        fprintf(stderr, "No bitcoin balances file found.\n");
        return 1;
    }

    // Read the file once to get the number of wallets and bitcoins:
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, sizeof(buffer), bitcoinBalancesFile))
    {
        strtok(buffer, " ");
        numberOfWallets++;
        while(strtok(NULL, " "))
        {
            numberOfBitcoins++;
        }
    }

    // We want to take the bucket size, get how many pointers can fit in it, reduce by 2
    // as the last 2 positions are special and then use that to divide the number of "things" we
    // want to store. This way we can have a number of entries for the hash table that ensures that
    // most places have just one bucket.
    walletHashTableNumOfEntries = (long int)(numberOfWallets / ((bucketSize / (sizeof(void*))) - 2));
    bitcoinHashTableNumOfEntries = (long int)(numberOfBitcoins / ((bucketSize / (sizeof(void*))) - 2));

    int minValue = (int)((bucketSize / (sizeof(void*))) / 5);

    if(walletHashTableNumOfEntries < minValue)
        walletHashTableNumOfEntries = minValue;
    if(bitcoinHashTableNumOfEntries < minValue)
        bitcoinHashTableNumOfEntries = minValue;

    // Initialize the hash tables with the number of buckets we calculated above:
    walletHashTable = initializeHashTable(walletHashTableNumOfEntries, bucketSize);
    bitcoinHashTable = initializeHashTable(bitcoinHashTableNumOfEntries, bucketSize);

    // Go back to beginning of file:
    rewind(bitcoinBalancesFile);

    // Read the file a second time and now add the wallets and bitcoins to the hash tables:
    while (fgets(buffer, sizeof(buffer), bitcoinBalancesFile))
    {
        // Get the wallet:
        walletID = strtok(buffer, " ");
        Wallet* newWallet = initializeWallet(walletID);
        inserted = insertToWalletHashTable(walletHashTable, newWallet,
            walletID, walletHashTableNumOfEntries);

        // If wallet already in hash table:
        if(inserted != 0)
        {
            fprintf(stderr, "Wallet ID duplicate found. May we see each other again.\n");
            return 1;
        }

        char* token = strtok(NULL, " ");
        int createLList = 0;
        while(token)
        {
            bitcoinID = atoi(token);
            BitcoinNode* newBitcoinNode = initializeBitcoin(NULL, newWallet, bitcoinValue);
            BitcoinRoot* newBitcoinRoot = initializeBitcoinRoot(bitcoinID, newBitcoinNode);
            Node* newBitcoinListNode = initializeNode(newBitcoinRoot);

            // If it's the first bitcoin ID, create a linked list in the wallet:
            if(createLList == 0)
            {
                LinkedList* newLinkedList = initializeLinkedList(newBitcoinListNode);
                newWallet->bitcoins = newLinkedList;
                newWallet->balance = bitcoinValue;
                createLList++;
            }
            // Else, append the new bitcoin ID to the existing linked list in the wallet:
            else
            {
                appendToLinkedList(newWallet->bitcoins, newBitcoinListNode);
                newWallet->balance += bitcoinValue;
            }

            inserted = insertToBitcoinHashTable(bitcoinHashTable, newBitcoinRoot,
            token, bitcoinHashTableNumOfEntries);

            // If bitcoin already in hash table:
            if(inserted != 0)
            {
                fprintf(stderr, "BitcoinID duplicate found. Goodbye for now my love.\n");
                return 1;
            }
            token = strtok(NULL, " ");
        }
    }

    return 0;
}

void readTransactionsFile(char* transactionFileName, HashTable* senderHashTable,
        HashTable* receiverHashTable, int senderHashTableSize, int receiverHashTableSize)
{
    FILE* transactionFile = fopen(transactionFileName, "r");

    int transactionID;
    char* senderWallet;
    char* receiverWallet;
    int value;
    char* date;
    char* time;

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