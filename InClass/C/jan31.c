#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv{

	/* Let's think about doubly-indexed arrays */


	int ** matrix;

	int rows, cols;

	rows = 2;
	cols = 3;


	/* First dimension is rows! */
	matrix = (int**) malloc( rows * sizeof(int*));

	int i, k;
	for ( i = 0; i < rows; i++){
		matrix[i] = (int*) malloc(cols * sizeof(int));
		
		for ( j = 0; j < cols; j++){
			scanf("%d", &( matrix[i][j] ) );
		}
	}

	/* Do stuff! */

	for ( i = 0; i < rows; i ++){
		for ( j = 0; j < cols; j++){
			printf("%5d ", matrix[i][j]);
		}
		printf("\n");
	}

	/* Free everything! */

	for ( i = 0; i < rows; i ++){
		free(matrix[i]);
		matrix[i] = 0;
	}

	free(matrix);
	matrix = 0;

	






	char* buffer = (char*) malloc(atoi(argv[1]));
	/* Malloc(size) gives a chunk of 'size' bytes */

	/*Go get a string, put it in the sequence of bytes starting at 'buffer'*/
	scanf("%s", buffer);
	/* INSTEAD: */

		fgets(buffer, 100, stdin); //Much getting than scanf w/ %s

	
	/* buffer[i] gives the ith entry of the array */

	int n = 10;	
	float* numbers = (float)* malloc(n * sizeof(float));


	printf("%llx\n",numbers);
	printf("%llx\n",&numbers);


	for (i = 0; i < n; i++){
		scanf("%f", &(numbers[i]));
	}

	for (i = 0; i < n; i++){
		printf("%07.4f\n", numbers[i]);
		/* 0 - Leadering 0's
 *		   7 - 7 total digits
 *		   4 - 4 #'s after decimal.
 */		
	}

	free(numbers); /* Need a free to match every malloc call!! */	



	return 0;
}
