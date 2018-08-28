#include <iostream>
#include <stdio.h>
#include <unistd.h>
using namespace std;

int main(int argc, char* argv[]){
     /* argc -- number of arguments */
     /* argv -- an array of strings */

	char* arg_buff[argc];
	int i;

	for (int i = 0; i < argc; i++){
		printf("argv[%d]: %s\n", i, argv[i]);
	}

	//create a buffer of arguments to be fed to command to be called
	//arg_buff[0] = argv[1];
	//arg_buff[1] = argv[2];
	//arg_buff[2] = NULL;

	//note that argc - 1 will be the last element of the array of input args
	for (i = 0; i < (argc - 1); i++){
		//here we assign arg_buff[i] with argv[i+1] to capture every element
		//after the initial program call of ./main
		arg_buff[i] = argv[i+1];
	}
	//null terminate the buffer
	arg_buff[i] = NULL;

	printf("Running execv...\n");
	//note that argv[1] is the desired command to be run from the cli
	execvp(argv[1], arg_buff);
	perror("execv");

	return 0;
}