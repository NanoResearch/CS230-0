#include "ComplexData.hpp"
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Sort.hpp"

int main(int argc, char** argv)
{
	int N = atoi(argv[1]);

	srand(time(NULL));

	Comparable** compare_list = (Comparable**)malloc(N* sizeof(Comparable*));
	Printable** print_list = (Printable**)compare_list;

	for ( int i = 0; i < N; i++ )
	{
		compare_list[i] = new ComplexData( double(rand()%100)/10.0, double(rand()%100)/10.0);
		print_list[i]->print();
	}

	sort(compare_list, N);

	for ( int i = 0; i < N; i++ )
	{
		print_list[i]->print();
	}

	for ( int i = 0; i < N; i++ )
	{
		delete compare_list[i];
	}

	free(compare_list);
	compare_list = 0;
	print_list = 0;
	
	return 0;
}
