// Everything we need to verify input.
#ifndef ERGASIA_1_VERIFICATION_H
#define ERGASIA_1_VERIFICATION_H
int allParametersGiven(unsigned long int bitCoinValue, char* bitCoinBalancesFile,
        char *transactionsFile, unsigned long senderHashtableNumOfEntries,
        unsigned long receiverHashtableNumOfEntries, unsigned long bucketSize);
#endif //ERGASIA_1_VERIFICATION_H
