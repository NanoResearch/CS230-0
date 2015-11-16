#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/** Real
 *
 * Take a list of numbers, and store them in an aarray.
 *
 * The list ends with either EOF, or a negative number.
 *
 * Do something! TODO: Compute max, min, average, media...
 *
 */


int main(int argc, char** argv){

	int size = 10;
	int count = 0;

	int* numbers = (int*) malloc( size * sizeof(int) );
	
	bzero(number, size*sizeof(int));	
	/* Fills everything with zeros from the get go */

	int i, j;

	while (1){
		int in;

		/* Reading int into in */
		int result = scanf("%d", &in);
		/* Result stores number of succesful reads */
		if (result == 0){
			break;
		}
		
		if ( in < 0 ){
			break;
		}

		/* Is the array almost full? */
		/* If we are out of room, make more */

		if (count >= size){
			/* Increase the size! */
			size += 10;
			numbers = (int*) realloc( numbers, size * sizeof(int) );			/* Take pointer to numbers and make it larger */
			/* Pointer location may change if necessary */
			/* You set this equal so that your data is preserved */ 	
		for ( j = count; j < size; j+){
			numbers[j] = 0;
		}

		}

		numbers[count] = in;
		count ++;
	}

	/* End of loop */

		/* Let's go all the way for the gigs */
	for ( i = 0; i < size; i++){
		printf("%d",numbers[i]);
	}

	printf("\n");
	free(numbers);
	numbers = 0;
	return 0;
}
