/* bw.c: Black and White conversion
 * This program takes an input image and averages the RGB data
 * to output a black and white file
 * By: Paul Gerlich
 */

#include<stdio.h>
#include<stdlib.h>
#include "lap.h"

void help()
{
printf("----------------------------------------------\n");
printf("-------------- Having troubles? --------------\n");
printf("-------- Here's how to work the program! -----\n");
printf("---- ./crop input.smp output.smp x y w h -----\n");
printf("------- input: Input image to be cropped -----\n");
printf("------- output: output image destination -----\n");
printf("----------------------------------------------\n");
printf("----------------- Have fun! ------------------\n");
}


struct image averageRGB(struct image in)
{
	struct image out;
	int i;
	unsigned char avgRGB;	
	
	printf("Averaging RGB data..\n");

	out.width = in.width;
	out.height = in.height;
	out.totalPixels = out.width * out.height;

	out.pixels = (struct pixel*) malloc(sizeof(struct pixel) * out.totalPixels);

	for ( i = 0; i < out.totalPixels; i++){
		avgRGB = ( (in.pixels[i].r + in.pixels[i].g + in.pixels[i].b ) / 3 );
		out.pixels[i].r = avgRGB;
		out.pixels[i].g = avgRGB;
		out.pixels[i].b = avgRGB;
		out.pixels[i].a = in.pixels[i].a;
	}

	return out;
}


int main(int argc, char** argv)
{
	FILE* infile;
	FILE* outfile;
	struct image inputImage, outputImage;


	if ( argc == 1 ){
		help();
		exit(0);
	}

	if ( argc < 3 ){
		printf("You are missing relevant arguments!\n");
		exit(0);	
	} else if ( argc > 3 ){
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

	/* Process the image */
	inputImage = process(infile);

	/* Average the RGB data and black and white the image */
	outputImage = averageRGB(inputImage);

	/* Export the image to SMP */
	export(outputImage, outfile);

	printf("Image has been succesfully converted to Black and White.\n");

	/* Free everything we maloc'd */
	free(inputImage.pixels);
	free(outputImage.pixels);
	
	/* Close our files up */
	fclose(infile);
	fclose(outfile);
	
	return 0;
}
