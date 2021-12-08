//Student Name; Hedaya Khalif
//Student #: 250887811
//CS3305 Assignment 3

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

int x, y, z;

void *sum(){
    z=y+x;
    return NULL;
}

int main(){
    pid_t child;
    pthread_t thread;

    // Initializing the global variables
    x=10, y=20, z=0;

    //printf("main function: errno number is %d\n", errno);


    // create child process
    child = fork();

    // check for child creation error
    if (child < 0){
        printf("Fork unsuccessful.");
        exit(1);
    }

        // in parent:
    else if (child > 0){
        // wait for child, and print value of z
        wait(NULL);
        //value of z after the fork process won't change
        printf("Using a fork(), the value of z in the parent process is: %d\n", z);

        // create thread, wait for it to complete, then print value of z
        if(pthread_create(&thread, NULL, &sum, NULL) != 0) {
            printf("main function: errno number is %d\n", errno);
            exit(1);}

        //terminate thread
        pthread_join(thread, NULL);
        //value of z after passing to the thread will change
        printf("Using a thread, the value of z is: %d\n", z);
    }

        // in child:
    else if (child == 0){
        // sum x and y and store it in z
        z = x + y;
        // print value of z
        printf("Using a fork(), the value of z in the child process is: %d\n", z);
    }


    return 0;
}
