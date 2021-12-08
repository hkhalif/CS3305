//Student Name: Hedaya Khalif
//Student Number: 250887811
//CS3305 Assignment 1


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv)
{

	// If the path of external_program.out isn't provided by the user, the program will terminate
	if (argc != 2)
	{
		printf("Invalid arguments\n");
		exit(0);
	}

	//creates parent and child processes
	pid_t parent, child_2;
    
    

	//creates child 1
	parent = fork();

	//waits until child 1 is done to create child 2
	wait(NULL);

	//creates child 2
	child_2 = fork();

	//unsuccessful fork
	if (parent < 0 || child_2 < 0){
		printf("fork unsuccessful");
		exit(1);
	}

	//parent
	else if (parent > 0 && child_2 > 0){

		//wait until child processes are complete
		wait(NULL);
	}

	//child 1
	else if (parent == 0 && child_2 > 0){
		printf("parent process (PID %d) created child_1 (PID %d) \n", getppid(), getpid());
		printf("parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2\n", getppid(), getpid());
	}

	//child 1.1
	else if(parent == 0 && child_2 == 0){
		printf("child_1 (PID %d) created child_1.1 (PID %d)\n", getppid(), getpid());
		printf("child_1 (PID %d) is now complete\n", getppid());
	}

	//child 2
	else{
        
        printf("parent (PID %d) created child_2 (PID %d)\n", getppid(), getpid());
		printf("child_2 (PID %d) is calling an external program external_program.out and leaving child_2..\n", getpid());
    
        
		//call external program
        execl("external_program.out",argv[1], NULL);
	}

	return 0;
}
