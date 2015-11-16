/*
 * Copying files
 *
 * cope file1.txt file2.txt
 *
 * will open file1, and copy it to file 2
 */


#include<stdio.h>

int main(int argc, char** argv){

	/*TODO: Double-check the arguments are the right kind! */

	FILE* infile = fopen(argv[1], "rb");

	if (infile == 0){
		printf("File %s failed to open!\n", argv[1]);
		return 1;
	}

	FILE* outfile = fopen(argv[2], "wb" );


	if (outfile == 0)
		printf("File %S failed to open!\n", argv[2]);
		fclose(infile);
		return 1;
	}

/* Can now asume infile and outfile are properly opened! */

#ifdef TEXTONLY
char c = fgetc(infile);
while (c != EOF){
	c = fgetc(infile);
	fputc(c, outfile);
}
#endif


while (feof(infile) ){
	char c = fgetc(infile);
	fputc(c, outfile);
}



