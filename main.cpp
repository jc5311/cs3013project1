// This main.cpp file creates a program that allows a user to input and execute
// UNIX comamands (like the cli would). However, processing is done around these
// commands utilizing the action of forking processes.

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
using namespace std;

/* **** FUNCTIONS **** */

//get system stats and output them
void getStats(){
	struct rusage usage;

	if (getrusage(RUSAGE_SELF, &usage ) < 0){
		cerr << "getrusage error";
	}

	//print data
	cout << "*** System resource statistics ***\n";
	cout << "User CPU time: " << ((usage.ru_utime.tv_sec * 1000) +
									(usage.ru_utime.tv_usec / 1000))
									<< "ms\n";
	cout << "System CPU time: " << ((usage.ru_stime.tv_sec * 1000) +
									(usage.ru_stime.tv_usec / 1000))
									<< "ms\n";
	cout << "Wall clock still needed!!!\n";
	cout << "Involuntary preemptions: " << usage.ru_nivcsw << "\n";
	cout << "Voluntary preemptions: " << usage.ru_nvcsw << "\n";
	cout << "Major page faults: " << usage.ru_majflt << "\n";
	cout << "Minor page faults: " << usage.ru_minflt << "\n";
	cout << "Max resident set size used: " << usage.ru_maxrss << "kB\n";


}



/* **** MAIN **** */
int main(int argc, char* argv[]){
     /* argc -- number of arguments */
     /* argv -- an array of strings */

	char* arg_buff[argc]; //buffer to store input args
	int i; //counter used in looping through input args
	int pid;

	//print the inputted cli args
	for (int i = 0; i < argc; i++){
		printf("argv[%d]: %s\n", i, argv[i]);
	}

	//store the args in a null terminated buffer
	//note that argc - 1 will be the last element of the array of input args
	for (i = 0; i < (argc - 1); i++){
		//here we assign arg_buff[i] with argv[i+1] to capture every element
		//after the initial program call of ./main
		arg_buff[i] = argv[i+1];
	}
	//null terminate the buffer
	arg_buff[i] = NULL;

	//perform command execution in a child process (do not replace the parent)
	if ( (pid = fork()) < 0){
		//if a problem occured when attempting to fork
		cerr << "Fork error\n";
		exit(1);
	}
	else if (pid == 0){
		//child process, run command
		cout << "Running execv...\n";
		//note that argv[1] is the desired command to be run from the cli
		execvp(argv[1], arg_buff);
		perror("execv");
	}
	else{
		//process must be the parent... do nothing for now.
	}

	wait(0);
	//sleep(5);
	getStats();
	printf("Done...\n");
	return 0;
}

