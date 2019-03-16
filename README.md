How to run the simulation:

1. First run make.
2. Then use the command:

./bitcoinSimulation -a bitCoinBalaceFile.txt -t transactionsFile.txt -v 50000 -h1 40 -h2 40 -b 1024

The parameters are the same as the ones given in the project description.

Explanation:
I'm using generic hash tables and linked lists with void pointers. Every bucket of the hashtable has a certain size(given in the beginning)and that size is divided by the number of bytes that a void pointer needs (4 bytes in a 32bit system, 8 bytes in a 64bit system etc.) and thus, in the case of a 64bit system, every 8 bits is a different item in the bucket. A slight problem with this approach is that I'm using malloc and what malloc returns is different depending on the implementation, so a malloc(250) may return 256 bytes and the proper solution for complete contol over the bucket size is to write my own malloc function, but that's a bit too much effort I think and the current solution works best.
