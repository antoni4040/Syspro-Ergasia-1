// Created by Antonis Karvelas.
// This should have been much bigger, but most of the verification code ended
// up in the commands.c.
#include <stdlib.h>
#include "verification.h"

// Make sure that all the initial command line parameters are given correctly.
int allParametersGiven(unsigned long int bitCoinValue, char* bitCoinBalancesFile,
        char *transactionsFile, unsigned long senderHashtableNumOfEntries,
        unsigned long receiverHashtableNumOfEntries, unsigned long bucketSize)
{
    if(bitCoinValue != -1 && bitCoinBalancesFile != NULL && transactionsFile != NULL
    && senderHashtableNumOfEntries != -1 && receiverHashtableNumOfEntries != -1 &&
    bucketSize != -1)
    {
        return 1;
    }
    else
        return 0;
}
