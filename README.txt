How to run the simulation:

1. First run make.
2. Then use the command:

./bitcoinSimulation -a bitCoinBalaceFile -t transactionsFile -v 50000 -h1 40 -h2 40 -b 1024

The parameters are the same as the ones given in the project description.

Explanation:
I'm using generic hash tables and linked lists with void pointers.
Every bucket of the hashtable has a certain size(given in the beginning)and that size
is divided by the number of bytes that a void pointer needs (4 bytes in a 32bit system,
8 bytes in a 64bit system etc.) and thus, in the case of a 64bit system, every 8 bits
is a different item in the bucket and the last two places are reserved for an item counter
(I'm storing an int in a void* but if we can't do such things then why use C...) and the pointer
to the next bucket. A slight problem with this approach is that I'm
using malloc and what malloc returns is different depending on the implementation,
so a malloc(250) may return 256 bytes. The proper solution for complete contol
over the bucket size is to write my own malloc function, but that's a bit too much
effort I think and the current solution works very well.
The linked list is pretty simple, just a head and a tail, nothing fancy.

The sender and receiver hash tables store linked lists with the transactions.
The wallet hash table stores wallets.
Each wallet stores some basic information and a linked lists with bitcoin roots, one for each
bitcoin that belongs to this wallet, even partially.
The bitcoin hash table stores bitcoin roots.
Bitcoin roots have a bitcoinID and a bitcoin node, the actual root node of the bitcoin tree.
Each bitcoin node has some information and 2 child bitcoin nodes.

Every time a transaction happens, we check to see whether it's valid and then we go through the
sender bitcoins and alter their trees until we get the amount that we want. If we find that a
bitcoin in the sender wallet has no value any more (it was spent in previous transactions),
we simply remove it. That removal always happens from the head of the linked list. If the receiver
has earned parts of a new bitcoin, we create a new node and add it to the receiver linked list.

I'm not sure what else needs explaining, most of the code is input verification and hash table
traversals...