#include "transactions.h"
#include "wallets.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void printTransaction(Transaction* transaction)
{
    printf("%s %s %s %i %s\n", transaction->transactionID,
        transaction->senderWalletID->walletID,
        transaction->receiverWalletID->walletID, transaction->value,
        asctime(localtime(&transaction->datetime)));
}

void printTransactionList(LinkedList* transactionLinkedList)
{
    Node* transactionNode = transactionLinkedList->head;
    do
    {
        printTransaction((Transaction*)(transactionNode->item));
        transactionNode = transactionNode->next;
    }while(transactionNode != NULL);
}

void printBucket(Bucket* bucket, int bucket_index, size_t bucketSize)
{
    for(int i = 0; i < bucketSize / sizeof(void*) - 2; i++)
    {
        if(bucket[i] != NULL)
            printTransactionList((LinkedList*)bucket[i]);
    }
}

void printTransactionHashTable(HashTable* hashTable, int hashTableSize, size_t bucketSize)
{
    printf("tr\n");
    for(int i = 0; i < hashTableSize; i++)
    {
        Bucket* bucket = hashTable->buckets[i];
        printBucket(bucket, i, bucketSize);
        while(bucket[bucketSize / sizeof(void*) - 1] != NULL)
        {
            bucket = (Bucket*)bucket[bucketSize / sizeof(void*) - 1];
            printBucket(bucket, i, bucketSize);
        }
    }
}

// Create a transaction:
Transaction* initializeTransaction(char* transactionID, Wallet* senderWallet,
        Wallet* receiverWallet, int value, char* date, char* _time)
{
    Transaction* newTransaction = malloc(sizeof(Transaction));
    newTransaction->transactionID = malloc(strlen(transactionID) * sizeof(char));
    strcpy(newTransaction->transactionID, transactionID);
    newTransaction->senderWalletID = senderWallet;
    newTransaction->receiverWalletID = receiverWallet;
    newTransaction->value = value;

    // Convert date and time into timestamp. Much easier to handle:
    struct tm timestruct;
    char datetime[30];
    strcpy(datetime, date);
    strcat(datetime, _time);
    strptime(datetime, "%d-%m-%Y%H:%M", &timestruct);
    newTransaction->datetime = mktime(&timestruct);

    return newTransaction;
}

// Return place of wallet id in bucket or -1 if it's not there:
int checkWalletIDInBucket(char* walletID, Bucket* bucket, size_t bucketSize, int walletType)
{
    int found = -1;
    for(int i = 0; i < bucketSize / sizeof(void*); i++)
    {
        if(bucket[i] == NULL)
            break;
        // Wallet type 1 for sender, 0 for receiver:
        if(walletType == 1)
        {
            if (strcmp(((Transaction*)(((LinkedList*)bucket[i])->head->item))->senderWalletID->walletID, walletID) == 0) {
                found = i;
                break;
            }
        }
        else
        {
            // printf("%li\n", ((Transaction*)(((LinkedList*)bucket[i])->head->item))->receiverWalletID);
            if (strcmp(((Transaction*)(((LinkedList*)bucket[i])->head->item))->receiverWalletID->walletID, walletID) == 0) {
                found = i;
                break;
            }
        }
    }
    return found;
}

// Add a new item in a hash table:
void insertToTransactionHashTable(HashTable* hashTable, Transaction* transaction,
        char* keyToHash, int walletIDType)
{
    int hashTableSize = hashTable->size;
    // Package the transaction in a transaction node:
    Node* newTransactionNode = initializeNode(transaction);
    int index = hash_function(keyToHash, hashTableSize);

    // Go through buckets and see if a list exists:
    size_t bucketSize = hashTable->bucketSize;
    Bucket* bucketToInsert = hashTable->buckets[index];
    int position = -1;
    do
    {
        position = checkWalletIDInBucket(keyToHash, bucketToInsert, bucketSize, walletIDType);
        if(position != -1)
            break;
        bucketToInsert = (Bucket*)bucketToInsert[(bucketSize / sizeof(void*)) - 1];
    }while(bucketToInsert != NULL);

    // If wallet is not found, we have to create new list in a bucket:
    if(position == -1)
    {
        bucketToInsert = hashTable->buckets[index];
        while(bucketToInsert[(bucketSize / sizeof(void*)) - 1] != NULL)
            bucketToInsert = (Bucket*)bucketToInsert[(bucketSize / sizeof(void*)) - 1];
        LinkedList* newTransactionList = initializeLinkedList(newTransactionNode);
        insertToBucket(bucketToInsert, newTransactionList, bucketSize);
    }

    // If wallet is already in a bucket, then add transaction to the list:
    else
    {
        appendToLinkedList((LinkedList*)bucketToInsert[position], newTransactionNode);
    }

}

// Check if the requested transaction is possible and do it. Engage.
int requestTransaction(Transaction* transaction, HashTable* walletHashTable,
    HashTable* senderHashTable, HashTable* receiverHashTable,
    int bitcoinValue, time_t* latestTransactionTime)
{
    Wallet* sender = transaction->senderWalletID;
    Wallet* receiver = transaction->receiverWalletID;
    int amount = transaction->value;

    printf("\n");
    printf("Requesting transaction:\n");
    printTransaction(transaction);
    printf("\n...\n");

    printf("Before:\n");
    printWallet(sender);
    printWallet(receiver);

    // Check sender and receiver are different:
    if(sender == receiver)
    {
        fprintf(stderr, "Sender and receiver can't be the same. What is this, money laundering?\n");
        return 1;
    }

    // Check transaction date:
    if(*latestTransactionTime >= transaction->datetime)
    {
        fprintf(stderr, "Date-time error. Do you have a TARDIS or something?\n");
        return 1;
    }

    // Check sender has enough money:
    if(amount > sender->balance)
    {
        fprintf(stderr, "Sender wallet doesn't have enough money. Just like me, actually...\n");
        return 1;
    }

    // Change the latest transaction time:
    *latestTransactionTime = transaction->datetime;

    // We can set the new balance here, as we're sure the transaction will work.
    sender->balance -= amount;
    receiver->balance += amount;

    // If amount is greater than the bitcoin value, take the first bitcoin from the
    // linked list in the sender and add it to the receiver:
    Node* bitcoinNodeToTake = sender->bitcoins->head;
    Node* next = bitcoinNodeToTake;
    Node** previous = NULL;
    while(amount > 0)
    {
        BitcoinRoot* bitcoinToTake = ((BitcoinRoot*)(bitcoinNodeToTake->item));
        // printf("%i\n", bitcoinToTake->bitcoinID);
        // Have to go through the God-damn tree:
        int amountGained = TreeBFSTransaction(bitcoinToTake, transaction, amount);
        // printf("Gained: %i\n", amountGained);
        amount -= amountGained;
        // Check to see whether bitcoin already in receiver:
        Node* node = receiver->bitcoins->head;
        int found = 0;
        while(node != NULL)
        {
            if(((BitcoinRoot*)(node))->bitcoinID == bitcoinToTake->bitcoinID)
            {
                found = 1;
                break;
            }
            node = node->next;
        }

        // If we don't get anything from this bitcoin, then the wallet no longer
        // has any portion of this bitcoin. Let's remove it:
        if(amountGained == 0)
        {
            if(sender->bitcoins->head == bitcoinNodeToTake)
            {
                Node** firstNode = &(sender->bitcoins->head);
                if(sender->bitcoins->tail == bitcoinNodeToTake)
                {
                    sender->bitcoins->head = NULL;
                    sender->bitcoins->tail = NULL;
                }
                else
                {
                    sender->bitcoins->head = sender->bitcoins->head->next;
                }
                free(*firstNode);
            }
            else
            {
                free((*previous)->next);
                (*previous)->next = bitcoinNodeToTake->next;
            }
        }

        Node* next = bitcoinNodeToTake->next;
        previous = &bitcoinNodeToTake;

        bitcoinNodeToTake = next;

        if(found == 1)
            continue;

        Node* newNode = initializeNode(bitcoinToTake);
        appendToLinkedList(receiver->bitcoins, newNode);
    }

    printf("After:\n");
    printWallet(sender);
    printWallet(receiver);

    insertToTransactionHashTable(senderHashTable, transaction, sender->walletID, 1);
    insertToTransactionHashTable(receiverHashTable, transaction, receiver->walletID, 0);
    return 0;
}