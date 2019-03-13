#include "commands.h"

void commandLine(HashTable* senderHashTable, HashTable* receiverHashTable,
    HashTable* walletHashTable)
{
    char* command;

    char* line;
    char* rest;
    size_t len = 0;

    putchar('~');
    putchar(' ');
    while(getline(&line, & len, stdin) != EOF)
    {
        line = strtok(line, "\n");
        command = strtok(line, " ");

        // If no input is given:
        if(command == NULL)
        {
            continue;
        }
        if(strcmp(command, "requestTransaction") == 0)
        {
            requestTransactionCommand(strtok(NULL, "\n"));
        }
        else if(strcmp(command, "exit") == 0)
        {
            break;
        }

        putchar('~');
        putchar(' ');
    }
}

void requestTransactionCommand(char* line)
{
    char* transactionID;
    char* sender;
    char* receiver;
    int value;
    char* date;
    char* time;
}

void walletStatusCommand(char* line)
{
    
}

// void findEarnings(HashTable* receivers, char* walletID, time_t start, time_t end)
// {
//     Wallet* wallet = findWalletInHashTable(receivers, walletID);
//     unsigned long int index = hash_function(walletID, receivers->size);
//     unsigned long int amount = 0;
//     Bucket* bucket = receivers->buckets[index];
//     int position = -1;
//     do
//     {
//         position = checkWalletIDInBucket(walletID, bucket, receivers->bucketSize, 0);
//         if(position != -1)
//             break;
//         bucket = (Bucket*)bucket[(receivers->bucketSize / sizeof(void*)) - 1];
//     }while(bucket != NULL);

//     if(position == 1)
//     {
//         fprintf(stderr, "Something went horribly wrong.\n");
//         return;
//     }

//     Node* transactionNode = ((Node*)(bucket[position]));
//     do
//     {
//         time_t datetime = ((Transaction*)(transactionNode->item))->datetime;
//         if(datetime >= start && datetime <= end)
//         {
//             amount += ((Transaction*)(transactionNode->item))->value;
//             transactionNode = transactionNode->next;
//         }
//     }while(transactionNode != NULL);

//     printf("Total amount received: %uli\n Now printing transactions:\n", amount);

//     Node* transactionNode = ((Node*)(bucket[position]));
//     do
//     {
//         time_t datetime = ((Transaction*)(transactionNode->item))->datetime;
//         if(datetime >= start && datetime <= end)
//         {
//             printTransaction((Transaction*)(transactionNode->item));
//             transactionNode = transactionNode->next;
//         }
//     }while(transactionNode != NULL);
// }