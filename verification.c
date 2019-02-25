// Everything we need to verify input.
#include <stdlib.h>
#include "verification.h"

// Make sure that all the initial command line parameters are given correctly.
int allParametersGiven(int bitCoinValue, char* bitCoinBalancesFile,
        char *transactionsFile, int senderHashtableNumOfEntries,
        int receiverHashtableNumOfEntries, int bucketSize)
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
