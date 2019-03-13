#include "commands.h"

void findEarnings(HashTable* receivers, char* walletID, time_t start, time_t end)
{
    Wallet* wallet = findWalletInHashTable(receivers, walletID);
    unsigned long int index = hash_function(walletID, receivers->size);
    unsigned long int amount = 0;
    Bucket* bucket = receivers->buckets[index];
    int position = -1;
    do
    {
        position = checkWalletIDInBucket(walletID, bucket, receivers->bucketSize, 0);
        if(position != -1)
            break;
        bucket = (Bucket*)bucket[(receivers->bucketSize / sizeof(void*)) - 1];
    }while(bucket != NULL);

    if(position == 1)
    {
        fprintf(stderr, "Something went horribly wrong.\n");
        return;
    }

    Node* transactionNode = ((Node*)(bucket[position]));
    do
    {
        time_t datetime = ((Transaction*)(transactionNode->item))->datetime;
        if(datetime >= start && datetime <= end)
        {
            amount += ((Transaction*)(transactionNode->item))->value;
            transactionNode = transactionNode->next;
        }
    }while(transactionNode != NULL);

    printf("Total amount received: %uli\n Now printing transactions:\n", amount);

    Node* transactionNode = ((Node*)(bucket[position]));
    do
    {
        time_t datetime = ((Transaction*)(transactionNode->item))->datetime;
        if(datetime >= start && datetime <= end)
        {
            printTransaction((Transaction*)(transactionNode->item));
            transactionNode = transactionNode->next;
        }
    }while(transactionNode != NULL);
}