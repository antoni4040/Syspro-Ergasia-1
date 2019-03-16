// Created by Antonis Karvelas.
// Everything about opening and reading data from files resides here.
#include "file_input.h"
#include "transactions.h"
#include "wallets.h"

#define BUFFER_SIZE 1024

int readBitcoinBalancesFile(char* bitcoinBalancesFileName, HashTable** walletHashTable,
        HashTable** bitcoinHashTable, size_t bucketSize, unsigned long int bitcoinValue)
{
    FILE* bitcoinBalancesFile = fopen(bitcoinBalancesFileName, "r");

    // We want to count the number of wallets and bitcoins first:
    unsigned long int numberOfWallets = 0;
    unsigned long int numberOfBitcoins = 0;
    unsigned long int walletHashTableNumOfEntries = 0;
    unsigned long int bitcoinHashTableNumOfEntries = 0;

    char* walletID;
    unsigned long int bitcoinID;
    int inserted;

    //Check bitcoin balances file name is correct:
    if(bitcoinBalancesFile == NULL)
    {
        fprintf(stderr, "No bitcoin balances file found.\n");
        return 1;
    }

    // Read the file once to get the number of wallets and bitcoins:
    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, bitcoinBalancesFile) != EOF)
    {
        strtok(line, " ");
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
    walletHashTableNumOfEntries = (unsigned long int)(
            numberOfWallets / ((bucketSize / (sizeof(void*))) - 2));
    bitcoinHashTableNumOfEntries = (unsigned long int)(
            numberOfBitcoins / ((bucketSize / (sizeof(void*))) - 2));

    unsigned long int minValue = (unsigned long int)((bucketSize / (sizeof(void*))) / 5);

    if(walletHashTableNumOfEntries < minValue)
        walletHashTableNumOfEntries = minValue;
    if(bitcoinHashTableNumOfEntries < minValue)
        bitcoinHashTableNumOfEntries = minValue;

    // Initialize the hash tables with the number of buckets we calculated above:
    *walletHashTable = initializeHashTable(walletHashTableNumOfEntries, bucketSize);
    *bitcoinHashTable = initializeHashTable(bitcoinHashTableNumOfEntries, bucketSize);

    // Go back to beginning of file:
    rewind(bitcoinBalancesFile);

    inserted = 0;

    // Read the file a second time and now add the wallets and bitcoins to the hash tables:
    while (getline(&line, &len, bitcoinBalancesFile) != EOF)
    {
        // Get the wallet:
        walletID = strtok(line, " ");
        Wallet* newWallet = initializeWallet(walletID);

        LinkedList* newLinkedList;
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
                newLinkedList = initializeLinkedList(newBitcoinListNode);
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

            inserted = insertToBitcoinHashTable(*bitcoinHashTable, newBitcoinRoot,
            token, bitcoinHashTableNumOfEntries);

            // If bitcoin already in hash table:
            if(inserted != 0)
            {
                free(newBitcoinListNode);
                free(newBitcoinNode);
                free(newBitcoinRoot);
                fprintf(stderr, "BitcoinID duplicate found. Goodbye for now my love.\n");
                break;
            }
            token = strtok(NULL, " ");
        }

        if(inserted != 0)
        {
            freeLinkedList(newLinkedList);
            freeWallet(newWallet);
            continue;
        }

        inserted = insertToWalletHashTable(*walletHashTable, newWallet,
            walletID, walletHashTableNumOfEntries);

        // If wallet already in hash table:
        if(inserted != 0)
        {
            freeWallet(newWallet);
            fprintf(stderr, "Wallet ID duplicate found. May we see each other again.\n");
            break;
        }
        // printWallet(newWallet);
    }

    free(line);
    fclose(bitcoinBalancesFile);
    return 0;
}

void readTransactionsFile(char* transactionFileName, HashTable* senderHashTable,
        HashTable* receiverHashTable, HashTable* walletHashTable,
        unsigned long int bitcoinValue, time_t* latestTransactionTime,
        unsigned long int* latestTransactionID)
{
    FILE* transactionFile = fopen(transactionFileName, "r");

    HashTable* transactionIDs = initializeHashTable(senderHashTable->size,
        senderHashTable->bucketSize);

    char* transactionID;
    unsigned long int transactionIDnum;
    char* sender;
    char* receiver;
    unsigned long value;
    char* date;
    char* _time;

    //Check transaction file name is correct:
    if(transactionFile == NULL)
    {
        fprintf(stderr, "No transaction file found.\n");
    }

    //Start reading the file line by line:
    char* line = NULL;
    size_t len = 0;
    while (getline(&line, &len, transactionFile) != EOF) {
        transactionID = strtok(line, " ");
        sender = strtok(NULL, " ");
        receiver = strtok(NULL, " ");
        value = strtoul(strtok(NULL, " "), NULL, 10);
        date = strtok(NULL, " ");
        _time = strtok(NULL, " ");

        unsigned long int index = hash_function(transactionID, transactionIDs->size);
        char* currTransactionID = NULL;
        int found = 0;
        Bucket* bucket = transactionIDs->buckets[index];
        while(bucket != NULL)
        {
            for(unsigned long int i = 0; i < (transactionIDs->bucketSize / sizeof(void*) - 2); i++)
            {
                currTransactionID = (char*)bucket[i];
                if(currTransactionID == NULL)
                    break;
                if(strcmp(currTransactionID, transactionID) == 0)
                {
                    found = 1;
                    break;
                }
            }
            bucket = (Bucket*)bucket[(transactionIDs->bucketSize / sizeof(void*) - 1)];
        }
        if(found == 1)
        {
            fprintf(stderr, "Transaction ID already exists. Moving on...\n");
            continue;
        }
        else
        {
            char* addTransactionID = malloc(strlen(transactionID) * sizeof(char));
            strcpy(addTransactionID, transactionID);
            bucket = transactionIDs->buckets[index];
            while(bucket[(transactionIDs->bucketSize / sizeof(void*)) - 1] != NULL)
                bucket = (Bucket*)bucket[(transactionIDs->bucketSize / sizeof(void*)) - 1];
            insertToBucket(bucket, addTransactionID, transactionIDs->bucketSize);
        }

        int isnum = 1;
        for(int i = 0; i < strlen(transactionID); i++)
        {
            if(isdigit(transactionID[i]))
            {
                continue;
            }
            else
            {
                isnum = 0;
                break;
            }
        }
        if(isnum == 1)
        {
            transactionIDnum = strtoul(transactionID, NULL, 10);
            if(transactionIDnum > *latestTransactionID)
                *latestTransactionID = transactionIDnum;
        }

        Wallet* senderWallet = findWalletInHashTable(walletHashTable, sender);
        Wallet* receiverWallet = findWalletInHashTable(walletHashTable, receiver);
        if(senderWallet == NULL)
        {
            fprintf(stderr, "Haven't seen this WalletID before. %s Who's this?\n", sender);
            continue;
        }
        if(receiverWallet == NULL)
        {
            fprintf(stderr, "Haven't seen this WalletID before. %s Who's this?\n", receiver);
            continue;
        }
        Transaction* newTransaction = initializeTransaction(transactionID, senderWallet, receiverWallet, value, date, _time);
        int transactionDone = requestTransaction(newTransaction, walletHashTable, senderHashTable, receiverHashTable,
            bitcoinValue, latestTransactionTime, 0);
        if(transactionDone == 1)
        {
            free(newTransaction->transactionID);
            free(newTransaction);
        }
    }

    for(unsigned long int i = 0; i < transactionIDs->size; i++)
    {
        Bucket* bucket = transactionIDs->buckets[i];
        while(bucket != NULL)
        {
            for(unsigned long int j = 0; j < (transactionIDs->bucketSize / sizeof(void*) - 2); j++)
            {
                char* stringID = (char*)bucket[j];
                if(stringID != NULL)
                    free(stringID);
            }
            Bucket* bucketToFree = bucket;
            bucket = (Bucket*)bucket[(transactionIDs->bucketSize / sizeof(void*) - 1)];
            free(bucketToFree);
        }
    }
    free(transactionIDs->buckets);
    free(transactionIDs);

    free(line);
    fclose(transactionFile);
}