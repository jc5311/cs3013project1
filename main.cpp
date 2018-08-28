#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

int main(int argc, char* argv[]){
     /* argc -- number of arguments */
     /* argv -- an array of strings */

	for (int i = 0; i < argc; i++){
		printf("argv[%d]: %s\n", i, argv[i]);
	}

	//create a buffer of arguments to be fed to command to be called
	char* arg_buff[argc];
	arg_buff[0] = argv[1];
	arg_buff[1] = argv[2];
	arg_buff[2] = NULL;

	printf("Running execv...\n");
	//note that argv[1] is the desired command to be run from the cli
	execvp(argv[1], arg_buff);
	perror("execv");

	return 0;
}