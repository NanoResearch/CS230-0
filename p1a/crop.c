/* Crop.c: This program takes in a 
 * picture and the X/Y starting position and
 * desired W/H Width/Height respectively to output
 * a cropped version of the image.
 * By: Paul Gerlich
 */


#include<stdio.h>
#include<stdlib.h>
#include "lap.h"

void help()
{
printf("----------------------------------------------\n");
printf("---------------Having troubles?---------------\n");
printf("---------Here's how to work the program!------\n");
printf("---- ./crop input.smp output.smp x y w h -----\n");
printf("------- input: Input image to be cropped -----\n");
printf("------- output: output image destination -----\n");
printf("------- X: x to start cropping at ------------\n");
printf("------- Y: y to start cropping at ------------\n");
printf("-- W: Desired final width of croppped image --\n");
printf("-- H: Desired final height of cropped image --\n");
printf("----------------------------------------------\n");
printf("--------------- Have fun! --------------------\n");
}


struct image crop (struct image in, int x, int y, int w, int h)
{
	struct image out;
	int i, j, k = 0, thisPixel = 0;
	
	printf("Cropping the image..\n");

	out.width = w;
	out.height = h;
	out.totalPixels = out.width * out.height;
	
	out.pixels = (struct pixel*) malloc(sizeof(struct pixel) * out.totalPixels);


	/* Iterate through all the pixels and keep which ones we want! */
	for ( i = 0; i < in.height; i++ ) {
		for ( j = 0; j < in.width; j++) {
			if ( j >= x && j < (x + w) ) {
				if ( i >= y && i < (y + h) ){
					out.pixels[k].r = in.pixels[thisPixel].r;
					out.pixels[k].g = in.pixels[thisPixel].g;
					out.pixels[k].b = in.pixels[thisPixel].b;
					out.pixels[k].a = in.pixels[thisPixel].a;
					k++;
				} 
			}
			thisPixel++;
		}
	}	
	
	return out;
}


int main(int argc, char** argv)
{
	int x, y, width, height;
	FILE* infile;
	FILE* outfile;
	struct image inputImage, outputImage;


	if ( argc == 1 ){
		help();
		exit(0);
	}

	if ( argc < 7 ){
		printf("You are missing relevant arguments!\n");
		exit(0);	
	} else if ( argc > 7 ){
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

	x = atoi(argv[3]);
	y = atoi(argv[4]);
	width = atoi(argv[5]);
	height = atoi(argv[6]);


	if (width <= 0){
		printf("You have specified an invalid width argument\n");
		exit(0);
	}
	
	if (height <= 0){
		printf("You have specified an invalid height argument\n");
		exit(0);
	}

	if (x < 0){
		printf("You have specified an x that is less than zero.\n");
		exit(0);
	}

	if (y < 0){
		printf("You have specified a y value that is less than 0.\n");
		exit(0);
	}

	/* Process the image */
	inputImage = process(infile);

	if (width > inputImage.width){
		printf("You have specified a width greater than the original file width.\n");
		exit(0);
	}
	if (height > inputImage.height){
		printf("You have specified a height greater than the original file height.\n");
		exit(0);
	}

	/* Crop the image */
	outputImage = crop(inputImage, x, y, width, height);

	/* Export the image to SMP */
	export(outputImage, outfile);

	printf("Image has been cropped successfully.\n");

	/* Freeing everything we malloc'd */
	free(inputImage.pixels);
	free(outputImage.pixels);

	/* Close our files */
	fclose(infile);
	fclose(outfile);
	
	return 0;
}
