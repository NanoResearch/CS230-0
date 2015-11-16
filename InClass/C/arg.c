/*
 * Argc.c: Output the command-line arguments
 *
 *
 * > cmd arg1 arg2 arg3 .... argn
 * if you include something like....
 * ./argc what\ is\ going\ on
 *
 * the output would be:
 * argc
 * What is going on?
 *
 * ^ Notice, the \ makes it ignore the space..!
 * */

#include<stdio.h>
#include<string.h>

int main(int argc, char** argv) {

	int i = 0;

	for (i = 0; i < argc; i++){
		if (argv[i][0] >= '0' && argv[i][0] <= '0') {
			int a = atoi(argv[i]);
			printf("%10d\n", a);
		} else {
			printf("%s\n", argv[i]);
		}
	}

	return 0;
}
