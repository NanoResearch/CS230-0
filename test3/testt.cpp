#include <stdlib.h>
#include <stdio.h>


void resizeAndZeroArray(int*& A, int newsize)
{
	if ( A == 0 )
	{
		A = (int*)malloc(newsize*sizeof(int));
	}
	else
	{
		A = (int*)realloc(A, newsize*sizeof(int));
	}

	/* zero out here */

}



int main(void)
{

	int i = 3;
	int j = 4;

	int& r = i, &t = j;


	int* p = &r;
	int* q = &t;

	*p = 6;
	*q = 8;

	/* what is i? */
	printf("%d %d", i, j);

	return 0;
}


