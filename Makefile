OBJS    =	main.o  commands.o  file_input.o  hashtable.o  linked_list.o  transactions.o verification.o  wallets.o
SOURCE  =	main.c  commands.c  file_input.c  hashtable.c  linked_list.c  transactions.c verification.c  wallets.c
HEADER  =	commands.h  file_input.h  hashtable.h  linked_list.h  transactions.h verification.h  wallets.h
OUT     =	bitcoinSimulation
CC      =	gcc
FLAGS   =       -Wall   -g      -c      -std=c99

$(OUT): $(OBJS)
	$(CC)   -g      $(OBJS) -o      $@

main.o: main.c
	$(CC)   $(FLAGS)        main.c

commands.o: commands.c
	$(CC)   $(FLAGS)        commands.c

file_input.o: file_input.c
	$(CC)   $(FLAGS)        file_input.c

hashtable.o: hashtable.c
	$(CC)   $(FLAGS)        hashtable.c

linked_list.o: linked_list.c
	$(CC)   $(FLAGS)        linked_list.c

transactions.o: transactions.c
	$(CC)   $(FLAGS)        transactions.c

verification.o: verification.c
	$(CC)   $(FLAGS)        verification.c

wallets.o: wallets.c
	$(CC)   $(FLAGS)        wallets.c

clean:
	rm -f $(OBJS) $(OUT)

count:
	wc $(SOURCE) $(HEADER)

