//Hedaya Khalif
//250887811
//CS3305 - Assignment2


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 25

int main(int argc, char **argv)
{

    // If the user does not pass X, Y and Z, the program will terminate
    if (argc != 4)
    {
        printf("Invalid arguments\n");
        exit(0);
    }

    //create pipe for parent to child communication
    char temp[20];
    int port[2];
    int status;
    status = pipe(port);

    //check that pipe was successfully created
    if (status == -1){
        printf("Pipe was not successful");
        return 1;
    }

    //create child
    pid_t parent = fork();

    //check that fork was successful
    if (parent < 0){
        printf ("Fork unsuccessful.");
        return 1;
    }

    //parent process
    if (parent > 0){

        printf("A pipe is created for communication between parent (PID %d) and child\n", getpid());

        //putting first agrument into variable x
        char *x = argv[1];
        printf("parent (PID %d) receives X = \"%s\" from the user\n", getpid(), x);

        //parent reading from pipe
        read(port[READ_END],temp,BUFFER_SIZE);
        printf("parent (PID %d) reads Y' from the pipe (Y' = \"%s\")\n", getpid(), temp);

        //concatenate x with temp, store into result
        char *result = strcat(x,temp);

        printf("parent (PID %d) concatenates X and Y' to generate the string: %s\n", getpid(), result);

        //process waits for child to complete before terminating parent
        wait(NULL);

    }

        //child process
    else if (parent == 0){
        printf("parent (PID %d) created a child (PID %d)\n", getppid(), getpid());

        //putting second and third arguments into variables y and z
        char *y = argv[2];
        char *z = argv[3];
        printf("child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user\n", getpid(), y, z);

        //child concatenates y and z to create y_prime
        char *y_prime = strcat(y,z);
        printf("child (PID %d) concatenates Y and Z to generate Y'= %s\n", getpid(), y_prime);

        //child writes y_prime into pipe
        write(port[WRITE_END],y_prime,BUFFER_SIZE);

        printf("child (PID %d) writes Y' into the pipe\n", getpid());
    }

    return 0;
}
