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
        else if(strcmp(command, "requestTransaction") == 0)
        {
            requestTransactionCommand(strtok(NULL, "\n"));
        }
        else if(strcmp(command, "findEarnings") == 0)
        {
            findEarningsPaymentsCommand(receiverHashTable, strtok(NULL, "\n"), 0);
        }
        else if(strcmp(command, "findPayments") == 0)
        {
            findEarningsPaymentsCommand(senderHashTable, strtok(NULL, "\n"), 1);
        }
        else if(strcmp(command, "walletStatus") == 0)
        {
            walletStatusCommand(strtok(NULL, "\n"), walletHashTable);
        }
        else if(strcmp(command, "exit") == 0)
        {
            break;
        }
        else
        {
            printf("You refuse to obey my command? Yes I do. It doesn't exist.\n");
        }

        putchar('~');
        putchar(' ');
    }
    free(line);
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

void walletStatusCommand(char* line, HashTable* walletHashTable)
{
    char* walletID = strtok(line, " ");
    if(walletID == NULL)
    {
        printf("No wallet ID given. Lost your wallet or something?\n");
        return;
    }
    Wallet* wallet = findWalletInHashTable(walletHashTable, walletID);
    if(wallet == NULL)
        printf("Wallet ID not found.\n");
    else
        printf("Wallet %s total balance: %lu\n", walletID, wallet->balance);
}

// For a given wallet print the total amount received and all the transactions
// related to it:
void findEarningsPaymentsCommand(HashTable* hashtable, char* line, int walletType)
{
    time_t start;
    time_t end;

    char* walletID = strtok(line, " ");
    char* parameter = strtok(NULL, " ");
    char* parameter2;
    char* parameter3;
    char* parameter4;

    struct tm timestruct;
    int day, month, year;
    int hours, minutes;
    int hoursEnd, minutesEnd;

    int timeGiven = 0;
    int yearGiven = 0;
    int allOfTime = 0;
    int timeOnly = 0;

    char datetime[30];

    if(walletID == NULL)
    {
        printf("Some parameters are missing. All of them actually...\n");
        return;
    }

    if(parameter == NULL)
    {
        allOfTime = 1;
    }
    else
    {
        // Read first parameter and check if it's a time or a date:
        if(sscanf(parameter, "%d-%d-%d", &day, &month, &year) == 3)
        {
            yearGiven = 1;
        }
        else if(sscanf(parameter, "%d:%d", &hours, &minutes) == 2)
        {
            // printf("%d %d\n", minutes, hours);
            timeGiven = 1;
        }
        else
        {
            printf("Time and date formats incorrect. The laws of time are mine! Or maybe not...\n");
            return;
        }

        parameter2 = strtok(NULL, " ");
        if(parameter2 == NULL)
        {
            printf("Give me some more parameters, I'm not an oracle...\n");
            return;
        }
        if(sscanf(parameter2, "%d-%d-%d", &day, &month, &year) == 3)
        {
            // If the second parameter is a date and the first is time, make a timestamp:
            if(timeGiven == 1)
            {
                yearGiven = 1;
                strcpy(datetime, parameter2);
                strcat(datetime, parameter);
                strptime(datetime, "%d-%m-%Y%H:%M", &timestruct);
                start = mktime(&timestruct);
            }
            else if(yearGiven == 1)
            {
                strcpy(datetime, parameter);
                strptime(datetime, "%d-%m-%Y", &timestruct);
                start = mktime(&timestruct);
                strcpy(datetime, parameter2);
                strptime(datetime, "%d-%m-%Y", &timestruct);
                end = mktime(&timestruct);
            }
            else
            {
                printf("Something went wrong.\n");
                return;
            }
        }
        else if(sscanf(parameter2, "%d:%d", &hoursEnd, &minutesEnd) == 2)
        {
            if(timeGiven == 1)
            {
                timeOnly = 1;
            }
        }
        else
        {
            printf("Time and date formats incorrect. The laws of time are mine! Or maybe not...\n");
            return;
        }
    }

    if(timeGiven == 1 && yearGiven == 1)
    {
        parameter3 = strtok(NULL, " ");
        parameter4 = strtok(NULL, " ");
        if(parameter3 == NULL || parameter4 == NULL)
        {
            printf("Parameters missing. Nuclear war starts in 3, 2, 1...\n");
            return;
        }

        if(sscanf(parameter3, "%d:%d", &hoursEnd, &minutesEnd) == 2)
        {
            timeGiven = 2;
        }
        else
        {
            printf("Parameters are in a sorry state. What are you trying to say?\n");
            return;
        }
        if(sscanf(parameter4, "%d-%d-%d", &day, &month, &year) == 3)
        {
            if(timeGiven == 2)
            {
                strcpy(datetime, parameter4);
                strcat(datetime, parameter3);
                strptime(datetime, "%d-%m-%Y%H:%M", &timestruct);
                end = mktime(&timestruct);
            }
            else
            {
                printf("Parameters are like my heart. Parts are missing...\n");
                return;
            }
        }
    }

    // FIND THE WALLET! (Imagine it an Uruk-Hai voice.)
    LinkedList* transactionList = findWalletInTransactionHashTable(hashtable, walletID, walletType);
    if(transactionList == NULL)
    {
        printf("Wallet not found. When you try your best but you don't succeed.\n");
        return;
    }

    // printf("%s\n", asctime(localtime(&start)));
    // printf("%s\n", asctime(localtime(&end)));

    unsigned long int amount = 0;

    // Go through the transactions:
    Node* transactionNode = transactionList->head;
    do
    {
        if(allOfTime == 1)
        {
            amount += ((Transaction*)(transactionNode->item))->value;
            transactionNode = transactionNode->next;
            continue;
        }
        time_t datetime = ((Transaction*)(transactionNode->item))->datetime;

        if(timeOnly == 1)
        {
            timestruct = *localtime(&datetime);

            timestruct.tm_min = minutes;
            timestruct.tm_hour = hours;
            timestruct.tm_sec = 0;
            start = mktime(&timestruct);

            timestruct.tm_min = minutesEnd;
            timestruct.tm_hour = hoursEnd;
            timestruct.tm_sec = 0;
            end = mktime(&timestruct);

            // printf("%s %s\n", asctime(localtime(&start)), asctime(localtime(&end)));
        }

        if(datetime >= start && datetime <= end)
        {
            amount += ((Transaction*)(transactionNode->item))->value;
        }
        transactionNode = transactionNode->next;
    }while(transactionNode != NULL);

    printf("Total amount received: %lu\n", amount);
    if(amount == 0)
        return;
    printf("Now printing transactions:\n");

    transactionNode = transactionList->head;
    do
    {
        if(allOfTime == 1)
        {
            printTransaction((Transaction*)(transactionNode->item));
            transactionNode = transactionNode->next;
            continue;
        }
        time_t datetime = ((Transaction*)(transactionNode->item))->datetime;

        if(timeOnly == 1)
        {
            timestruct = *localtime(&datetime);

            timestruct.tm_min = minutes;
            timestruct.tm_hour = hours;
            timestruct.tm_sec = 0;
            start = mktime(&timestruct);

            timestruct.tm_min = minutesEnd;
            timestruct.tm_hour = hoursEnd;
            timestruct.tm_sec = 0;
            end = mktime(&timestruct);

            // printf("%s %s\n", asctime(localtime(&start)), asctime(localtime(&end)));
        }

        if(datetime >= start && datetime <= end)
        {
            printTransaction((Transaction*)(transactionNode->item));
        }
        transactionNode = transactionNode->next;
    }while(transactionNode != NULL);
}