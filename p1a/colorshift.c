/* Colorshift.c: This program takes in a picture and a pattern argument
 * and switches the RGB values according to the pattern.
 * By: Paul Gerlich
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lap.h"

void help()
{
printf("----------------------------------------------\n");
printf("---------------Having troubles?---------------\n");
printf("---------Here's how to work the program!------\n");
printf("-- ./colorshift input.smp output.smp pattern -\n");
printf("------- input: Input image to be cropped -----\n");
printf("------- output: output image destination -----\n");
printf("------- Pattern: RGB RBG ect... - ------------\n");
printf("--- Where each pattern switches the colors ---\n");
printf("--- With the color to the right. Mind that ---\n");
printf("--- With three colors, the right most color --\n");
printf("--- Switches with the color on the left! -----\n");
printf("----------------------------------------------\n");
printf("--------------- Have fun! --------------------\n");
}


struct image colorShift(struct image in, char* pattern)
{
	struct image out;
	int i, num;
	
	printf("Shifting colors according to %s pattern..\n", pattern);

	out.width = in.width;
	out.height = in.height;
	out.totalPixels = out.width * out.height;

	out.pixels = (struct pixel*) malloc(sizeof(struct pixel) * out.totalPixels);
	
	num = strlen ( pattern );

	if ( num < 2 ) {
		printf("You specified too few colors in the pattern!");
		exit(0);
	} else if ( num > 3 ) {
		printf("You specified too many colors in the pattern!");
		exit(0);
	}

	for ( i = 0; i < out.totalPixels; i++) {
	
		/* Ugly way of checking which ones will be switched. */
		if (num == 2){
			/* We will be using nested switches to change out the values */
			switch(pattern[0]) {
				/* First is R */
				case 'R':
				case 'r':
					switch(pattern[1]) {
						/* Second is G */
						case 'G':
						case 'g':
							out.pixels[i].r = in.pixels[i].g;
							out.pixels[i].g = in.pixels[i].r;
							out.pixels[i].b = in.pixels[i].b;
							break;
						/* Second is B */
						case 'B':
						case 'b':
							out.pixels[i].r = in.pixels[i].b;
							out.pixels[i].b = in.pixels[i].r;
							out.pixels[i].g = in.pixels[i].g;
							break;
					}
					break;
				/* First is G */
				case 'G':
				case 'g':
					switch(pattern[1]) {
						/* Second is R */
						case 'R':
						case 'r':
							out.pixels[i].g = in.pixels[i].r;
							out.pixels[i].r = in.pixels[i].g;
							out.pixels[i].b = in.pixels[i].b;
							break;
						/* Second is B */
						case 'B':
						case 'b':
							out.pixels[i].g = in.pixels[i].b;
							out.pixels[i].b = in.pixels[i].g;
							out.pixels[i].r = in.pixels[i].r;
							break;
					}
					break;
				/* First is B */
				case 'B':
				case 'b':
					switch(pattern[1]) {
						/* Second is R */
						case 'R':
						case 'r':
							out.pixels[i].r = in.pixels[i].b;
							out.pixels[i].b = in.pixels[i].r;
							out.pixels[i].g = in.pixels[i].g;
							break;
						/* Second is G */
						case 'G':
						case 'g':
							out.pixels[i].b = in.pixels[i].g;
							out.pixels[i].g = in.pixels[i].b;
							out.pixels[i].r = in.pixels[i].r;
							break;
					}
					break;

				default:
					printf("I'm sorry, something went wrong along the way. Please try again!\n");
					exit(0);
			}
		/* If we have 3 colors to shift */
		} else {
			switch(pattern[0]) {
				/* First is R */
				case 'R':
				case 'r':
					switch(pattern[1]) {
						/* Second is G */
						case 'G':
						case 'g':
							out.pixels[i].r = in.pixels[i].b;
							out.pixels[i].g = in.pixels[i].r;
							out.pixels[i].b = in.pixels[i].g;
							break;
						/* Second is B */
						case 'B':
						case 'b':
							out.pixels[i].r = in.pixels[i].g;
							out.pixels[i].b = in.pixels[i].r;
							out.pixels[i].g = in.pixels[i].b;
							break;
					}
					break;
				/* First is G */
				case 'G':
				case 'g':
					switch(pattern[1]) {
						/* Second is R */
						case 'R':
						case 'r':
							out.pixels[i].g = in.pixels[i].b;
							out.pixels[i].r = in.pixels[i].g;
							out.pixels[i].b = in.pixels[i].r;
							break;
						/* Second is B */
						case 'B':
						case 'b':
							out.pixels[i].g = in.pixels[i].r;
							out.pixels[i].b = in.pixels[i].g;
							out.pixels[i].r = in.pixels[i].b;
							break;
					}
					break;
				/* First is B */
				case 'B':
				case 'b':					
					switch(pattern[1]) {
						/* Second is R */
						case 'R':
						case 'r':
							out.pixels[i].b = in.pixels[i].g;
							out.pixels[i].r = in.pixels[i].b;
							out.pixels[i].g = in.pixels[i].r;
							break;
						/* Second is G */
						case 'G':
						case 'g':
							out.pixels[i].b = in.pixels[i].r;
							out.pixels[i].g = in.pixels[i].b;
							out.pixels[i].r = in.pixels[i].g;
							break;
					}
					break;
					
				default:
					printf("I'm sorry, something went wrong along the way. Please try again!\n");
					exit(0);
			}
		}
		out.pixels[i].a = in.pixels[i].a;
	}

	return out;
}


int main(int argc, char** argv)
{
	int x, y, width, height;
	char* buffer;
	FILE* infile;
	FILE* outfile;
	struct image inputImage, outputImage;


	if ( argc == 1 ){
		help();
		exit(0);
	}

	if ( argc < 4 ){
		printf("You are missing relevant arguments!\n");
		exit(0);	
	} else if ( argc > 4 ){
		printf("You have too many arguments!\n");
		exit(0);
	}

	infile = fopen(argv[1],"r");
	if ( !infile ){
		printf("Unable to open the input file at %s\n",argv[1]);
		exit(0);
	}

	outfile = fopen(argv[2],"w");
	if ( !outfile ){
		printf("Unable to open the output at %s\n",argv[2]);
		printf("Perhaps it is already open, or you do not have permissions?\n");
		exit(0);
	} else if (strcmp(argv[1],argv[2]) == 0) {
		printf("You cannot open and modify the same file.\n");
		printf("Please specify a different output file name.\n");
		exit(0);
	}

	if ( strcmp ("RAGE",argv[3]) == 0 ){
		printf("You are the winner.\n");
		exit(0);
	}

	if ( strlen ( argv[3] ) > 3 ) {
		printf("You have specified a pattern that is too long!");
		exit(0);
	} else if ( strlen ( argv[3] ) < 2  ) {
		printf("You have specified a pattern that is too short!");
		exit(0);
	} 

	buffer = ( char* ) malloc( strlen( argv[3] ) + 1 );

	strcpy( buffer, argv[3] );

	/* Process the image */
	inputImage = process(infile);

	/* Pass the pattern and image to our swapRGB function to shift dem colors! */
	outputImage = colorShift(inputImage, buffer);

	/* Export the image to SMP */
	export(outputImage, outfile);

	printf("Image has been successfully shifted.\n");
	
	/* Free our malloc'd variables */
	free(buffer);
	free(inputImage.pixels);
	free(outputImage.pixels);
	
	
	/* Close our files */
	fclose(infile);
	fclose(outfile);

	return 0;
}
