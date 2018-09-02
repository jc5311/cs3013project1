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
#include <string.h>
using namespace std;


/* **** DEFINES **** */
#	define MAX_BUFFER_SIZE 500

/* **** GLOBAL VALUES **** */
string cli_prompt = "==>";

/* **** FUNCTIONS **** */
void getStats(){
	//get system stats and output them
	struct rusage usage;

	if (getrusage(RUSAGE_CHILDREN, &usage ) < 0){
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
	cout << "**********************************\n";
}

void dispPrompt(){
	cout << cli_prompt;
}

void changePrompt(string new_cli_prompt){
	cli_prompt = new_cli_prompt;
}

/* **** MAIN **** */
int main(int argc, char* argv[]){
     /* argc -- number of arguments */
     /* argv -- an array of strings */
	char* cli_buff[argc]; //buffer to store cli args
	char* token;
	char* shell_buff[MAX_BUFFER_SIZE];
	char* command;
	int i; //counter used in looping through input args
	int j; //counter used during string pasrsing
	int pid;
	int state = 0; // 0 = no work to do -> shell prompt; 1 = work to do
	int num_arg;
	string cli_input;

	if (argc > 1){
		state = 1;
	}

	while(1){
		//program loop
		if (state == 0){
			//begin shell and ask for input
			dispPrompt();
			getline(cin, cli_input);

			char* buffer = (char*) cli_input.c_str();
			token = strtok(buffer, " ");
			
			for (j = 0; token != NULL; j++){
				//save token to buffer
				shell_buff[j] = token;
				token = strtok(NULL, " ");
			}
			shell_buff[j] = NULL;

			//check for shell specific commands
			if (strstr(shell_buff[0], "exit") != NULL){
				exit(0);
			}else if (strstr(shell_buff[0], "cd") != NULL){
				chdir(shell_buff[1]);
				perror("chdir");
				continue;
			}


			//perform command execution in a child process (do not replace the parent)
			if ( (pid = fork()) < 0){
				//if a problem occured when attempting to fork
				cerr << "Fork error\n";
				exit(1);
			}
			else if (pid == 0){
				//child process, run command
				//note that argv[1] is the desired command to be run from the cli
				execvp(shell_buff[0], shell_buff);
				perror("execv");
				exit(0);
			}
			else{
				//parent process, do nothing.
				wait(0);
			}
			getStats();

		} else{
			//if arguments beyond the application call were made
			//then do work on them
			//this block should only occur on first execution of the application

			//print the inputted cli args
			for (int h = 0; h < argc; h++){
				printf("argv[%d]: %s\n", h, argv[h]);
			}

			//store the args in a null terminated buffer
			//note that argc - 1 will be the last element of the array of input args
			for (i = 0; i < (argc - 1); i++){
				//here we assign cli_buff[i] with argv[i+1] to capture every element
				//after the initial program call of ./main
				cli_buff[i] = argv[i+1];
			}
			//null terminate the buffer
			cli_buff[i] = NULL;

			//print the inputted cli args
			for (int h = 0; h < i; h++){
				printf("cli_buff[%d]: %s\n", h, cli_buff[h]);
			}

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
				execvp(cli_buff[0], cli_buff);
				perror("execv");
				exit(0);
			}
			else{
				//parent process, do nothing.
			}

			state = 0; //switch to shell
			wait(0);
			getStats();
		}
	} //end of while loop
return 1; //should no longer be reached
}

