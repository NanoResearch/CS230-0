/*
 *
 * Introduction to files 
 *
 * Specifics:
 * 	"R" = Read
 * 	"W" = Write
 * 	"rb" = Read Binary
 */

#include<stdio.h>

int main(int argc, char** argv) {


	FILE* infile = fopen("file.txt","r");
	FILE* outfile = fopen("file2.txt","w");

	FILE* binary = fopen("data.simp", "rb");

	if(infile == 0){
		/*Error occured while opening.. */
		printf("Failed to open file.txt!\n");
		/* TODO: Close other files. */
		return 1;
	}

	/* Flush the file buffer */
	fflush(outfile);


	/* Reading from the infile */
	int number = 0;
	fscanf(infile, "%d", &number); // Similar to scan f, for afile
	/* fscanf(stdin,format,args...) == scanf(format, args...) */


	/* To read a single int from a binary file: */
	/* fread(data pointer, wordsize, length, FILE*/
	fread(&number, sizeof(int), 1, binary);


	fprintf(outfile, "%04d\n", number);
	/* fprintf(stdout, format, args...) == printf(format, args...) */

	
	/* fwrite(data pointer, wordsize, length, FILE* ) */
	fwrite( &number, sizeof(int), 1, binary);

	char c = fgetc(infile); /* Get a single character from infile */

	if (c == EOF) {
		// Works for text files..

	}

	if (feof(binary)){
		// I get here if binary is out of data.. == EOF for normal
	}

	// Not recommended using fgets!!!

	fputc(outfile, c); /*Write a single character to outfile */

/* When done with file, must close.. */

	fclose(infile);
	fclose(outfile);
	fclose(binary);
	infile = 0;
	outfile = 0;
	binary = 0;
