//Hedaya Khalif
//250887811
//CS3305 Assignment 5/6

//include libraries
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>


//create bank account structure
struct bankAccount{
    //account number (ex.a1 would be 1)
    int number;
    int balance;

};

//create structure for transfer transaction
struct orderedPair{
    struct bankAccount x;
    struct bankAccount y;
};

//lock thread
sem_t mutex;

//structure for all accounts
struct bankAccount accounts[50];

//deposit transaction
struct bankAccount deposit (struct bankAccount baccount, int amount) {

    //lock
    sem_wait(&mutex);
    //add deposit amount to account balance
    baccount.balance += amount;
    //unlock
    sem_post(&mutex);

    return baccount;
}

//withdraw transaction
struct bankAccount withdraw (struct bankAccount baccount, int amount){

    //lock
    sem_wait(&mutex);

    //check if balance is greater than withdraw amount
    if (baccount.balance >= amount){
        //subtract withdraw amount from balance
        baccount.balance -= amount;

    }

    //unlock
    sem_post(&mutex);

    return baccount;
}

//transfer transaction
struct orderedPair transfer (struct bankAccount baccount1, struct bankAccount baccount2, int amount) {
    //lock
    sem_wait(&mutex);


    //check if account 1 balance is greater than amount being transfered
    if (baccount1.balance >= amount){
        //subtract amount from account 1
        baccount1.balance -= amount;
        //add amount to account 2
        baccount2.balance += amount;
    }

    //unlock
    sem_post(&mutex);

    struct orderedPair new;
    new.x = baccount1;
    new.y = baccount2;
    return new;

}

//prints into output file account number and balances
void print_output(struct bankAccount account){
    FILE *fptr;
    fptr = fopen("assignment_6_output_file.txt", "a");
    //prints balances to output file
    fprintf(fptr,"a%d b %d \n", account.number, account.balance);
    //prints balance onto screen
    printf("a%d b %d \n", account.number, account.balance);
}

//main program
int main(int argc, char* argv[]) {

    //check that correct number of arguments entered
    if (argc != 2){
        printf("Usage: %s input.txt\n", argv[0]);
        exit(1);
    }

    sem_init(&mutex, 1, 1);
    //remove("assignment_3_output_file.txt");
    int size = 0;
    FILE* fptr;
    //declares vars, ptr, and delim, char
    char delim[] = " ";
    //opens input file
    fptr = fopen(argv[1], "r");
    //check if error when opening file
    if (fptr == NULL){
        perror("Unable to open file");
        exit(1);
    }

    char line[256];

    //parse file line by line to read account numbers and balances
    while (fgets(line, sizeof(line), fptr) && (line[0] == 'a')){
        char *ptr = strtok(line, delim);
        accounts[size].number = size +1;
        ptr = strtok(NULL, delim);
        ptr = strtok(NULL, delim);
        accounts[size].balance = atoi(ptr);

        size++;
    }

    //close file
    fclose(fptr);
    //reopen file
    fptr = fopen("assignment_6_input_file.txt", "r");

    //parse file for customer transactions
    while (fgets(line, sizeof(line), fptr) && (line[0] != '\n')){

        //printf("%s",line);
        char *ptr = strtok(line, delim);
        int act1,act2, amt;
        while (ptr != NULL){
            ptr = strtok(NULL, delim);
            if (ptr == NULL){
                break;
            }

            //check for deposit transaction
            if (strcmp(ptr,"d")==0){
                ptr = strtok(NULL, delim);

                //checking for account for deposit
                if (strlen(ptr)==2){
                    char num[1] = "";
                    num[0] = ptr[1];
                    act1 = atoi(num);
                }else if (strlen(ptr)==3){
                    char num[2] = "";
                    num[0] = ptr[1];
                    num[1] = ptr[2];
                    act1 = atoi(num);
                }
                ptr = strtok(NULL, delim);

                //amount to be deposited put into amt variable
                amt = atoi(ptr);

                struct bankAccount newval;
                newval = deposit(accounts[act1-1],amt);
                accounts[act1-1] = newval;

            //checking for withdraw transaction
            }else if (strcmp(ptr, "w")==0){
                ptr = strtok(NULL, delim);

                //checking for account to be withdrawn from
                if (strlen(ptr)==2){
                    char num[1] = "";
                    num[0] = ptr[1];
                    act1 = atoi(num);
                }else if (strlen(ptr)==3){
                    char num[2] = "";
                    num[0] = ptr[1];
                    num[1] = ptr[2];
                    act1 = atoi(num);
                }
                ptr = strtok(NULL, delim);
                //amount to be withdrawn put into amt variable
                amt = atoi(ptr);
                struct bankAccount newval;
                newval = withdraw(accounts[act1-1],amt);
                accounts[act1-1] = newval;

             //checking for transfer transaction
            }else if (strcmp(ptr, "t")==0){
                ptr = strtok(NULL, delim);

                //checking for account from transfer to be taken from
                if (strlen(ptr)==2){
                    char num[1] = "";
                    num[0] = ptr[1];
                    act1 = atoi(num);
                }else if (strlen(ptr)==3){
                    char num[2] = "";
                    num[0] = ptr[1];
                    num[1] = ptr[2];
                    act1 = atoi(num);
                }

                //checking for account from transfer to be given to
                ptr = strtok(NULL, delim);
                if (strlen(ptr)==2){
                    char num[1] = "";
                    num[0] = ptr[1];
                    act2 = atoi(num);
                }else if (strlen(ptr)==3){
                    char num[2] = "";
                    num[0] = ptr[1];
                    num[1] = ptr[2];
                    act2 = atoi(num);
                }
                ptr = strtok(NULL, delim);

                //putting transfer amount into amt variable
                amt = atoi(ptr);

                struct orderedPair newv;
                newv =transfer(accounts[act1-1],accounts[act2-1],amt);
                accounts[act1-1] = newv.x;
                accounts[act2-1] = newv.y;
            }
        }
    }

    //unlock
    sem_destroy(&mutex);

    //close file
    fclose(fptr);

    //print out accounts with their balances after all transactions
    for (int i =0; i<=size-1; i++){
        print_output(accounts[i]);
    }
    return 0;
}
