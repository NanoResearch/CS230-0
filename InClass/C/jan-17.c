/*
 * The devil returns.. Miner sub day
 * 
 * In sum: Went over scanf not being applicable to strings due to it skipping
 * spaces.
 *
 * Then went over using loops and chararrays to read string input.
 * (see below)
 *
 * Then structs, and passing structs through functions as parameters.
 *
 * Started software modularization via functions to allow multiple calls to the function within the same runtime.
 *
 */

#include <stdio.h>


struct person {
	int age;
	char name[100];
};

struct person readPersonalStuff()
{

}

int main()
{
	printf("Hello, how old are you?\n");

	int age;
	scanf("%d", &age);

	char name[10000];

printf("What is your name?\n");

int i;
char dummy = getchar();

printf("Sanity check: This is newline? %d\n", dummy);

for (i=0;i<10000;i++){
	char c = getchar();
	if (c=='\n') break;
	name[i] = c;
}
name[i] = 0; // Simulates end of string character "Miners black magic"


int birthyear = current_year - age;
printf("Hello, %s, nice to meet you.\n", name);
printf("You were born in either %d or %d\n", birthyear, birthyear-1);

return 0;
}


