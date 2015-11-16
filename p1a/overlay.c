/* Overlay.c: This program takes in two pictures and overlays the second image
 * over the first image with the top left of the second image being at
 * the position X,Y
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
printf("./overlay input1.smp input2.smp output.smp x y\n");
printf("--- input1: Input image for the background ---\n");
printf("------- input2: Input image to be on top -----\n");
printf("------- output: output image destination -----\n");
printf("------- X: x to mark top left of overlay -----\n");
printf("------- Y: y to mark top left of overlay -----\n");
printf("----------------------------------------------\n");
printf("--------------- Have fun! --------------------\n");
}


struct image overlayImages(struct image bottom, struct image top, int x, int y)
{
	double r, g, b, a;
	int i, j, thisPixel = 0, k = 0;
	struct image out;
	
	printf("Overlaying images..\n");

	out.width = bottom.width;
	out.height = bottom.height;
	out.totalPixels = out.width * out.height;
	
	out.pixels = (struct pixel*) malloc(sizeof(struct pixel) * out.totalPixels);


	/* Iterate through all the pixels and keep which ones we want! */
	for ( i = 0; i < bottom.height; i++ ) {
		for ( j = 0; j < bottom.width; j++) {
			if ( (j >= x && j < (x + top.width) ) && ( i >= y && i < ( y + top.height ) ) ) {
					/* Overlay two images based on Alpha value of top and then calculates resulting pixels RGBA values */
					r = (double) ( ( top.pixels[k].a ) / 255.0 ) * top.pixels[k].r + ( ( ( bottom.pixels[thisPixel].a * ( 255.0 - top.pixels[k].a ) ) / ( 255.0 * 255.0 ) ) * bottom.pixels[thisPixel].r );
					g = (double) ( ( top.pixels[k].a ) / 255.0 ) * top.pixels[k].g + ( ( ( bottom.pixels[thisPixel].a * ( 255.0 - top.pixels[k].a ) ) / ( 255.0 * 255.0 ) ) * bottom.pixels[thisPixel].g );
					b = (double) ( ( top.pixels[k].a ) / 255.0 ) * top.pixels[k].b + ( ( ( bottom.pixels[thisPixel].a * ( 255.0 - top.pixels[k].a ) ) / ( 255.0 * 255.0 ) ) * bottom.pixels[thisPixel].b );
					a = (double) 255.0 * ( ( top.pixels[k].a / 255.0 ) + ( ( bottom.pixels[thisPixel].a * ( 255.0 - top.pixels[k].a ) ) / ( 255.0 * 255.0 ) ) );
					out.pixels[thisPixel].r = (unsigned char) r;
					out.pixels[thisPixel].g = (unsigned char) g;
					out.pixels[thisPixel].b = (unsigned char) b;
					out.pixels[thisPixel].a = (unsigned char) a;
					/* printf("We are getting R: %d G: %d B: %d A: %d\n", out.pixels[thisPixel].r, out.pixels[thisPixel].g, out.pixels[thisPixel].b, out.pixels[thisPixel].a); */ 
					k++; 
			} else {
					out.pixels[thisPixel].r = bottom.pixels[thisPixel].r;
					out.pixels[thisPixel].g = bottom.pixels[thisPixel].g;
					out.pixels[thisPixel].b = bottom.pixels[thisPixel].b;
					out.pixels[thisPixel].a = bottom.pixels[thisPixel].a;
			}
			thisPixel++;
		}
	}	

	return out;
}


int main(int argc, char** argv)
{
	int x, y;
	FILE* infileBottom;
	FILE* infileTop;
	FILE* outfile;
	struct image backImage, frontImage, outputImage;


	if ( argc == 1 ){
		help();
		exit(0);
	}

	if ( argc < 6 ){
		printf("You are missing relevant arguments!\n");
		exit(0);	
	} else if ( argc > 6 ){
		printf("You have too many arguments!\n");
		exit(0);
	}

	infileBottom = fopen(argv[1],"r");
	if ( !infileBottom ){
		printf("Unable to open the input file at %s\n",argv[1]);
		exit(0);
	}

	infileTop = fopen(argv[2], "r");
	if ( !infileTop ){
		printf("Unable to open the input file at %s\n",argv[3]);
		exit(0);
	}


	outfile = fopen(argv[3],"w");
	if ( !outfile ){
		printf("Unable to open the output at %s\n",argv[3]);
		printf("Perhaps it is already open, or you do not have permissions?\n");
		exit(0);
	}

	
	x = atoi(argv[4]);
	if ( x < 0 ){
		printf("You have specified a negative x value!\n");
		exit(0);
	}
	
	y = atoi(argv[5]);
	if ( y < 0 ){
		printf("You have specified a negative y value!\n");
		exit(0);
	}

	/* Process the two images */
	backImage = process(infileBottom);
	frontImage = process(infileTop);

	/* Throw an error after it's processed if the end user is attempting to overlay at a point that doesn't exist */
	if (x > backImage.width){
		printf("You have specified an X that was too large to fit on the background image!\n");
		exit(0);
	} 
	
	if (y > backImage.height){
		printf("You have specified a Y that was too large to fit on the background image!\n");
		exit(0);
	}

	/* Overlay the two images */
	outputImage = overlayImages(backImage, frontImage, x, y);

	/* Export the image to SMP */
	export(outputImage, outfile);

	printf("Images have been over laid successfully.\n");

	/* Free our maloc'd variables */
	free(backImage.pixels);
	free(frontImage.pixels);
	free(outputImage.pixels);
	
	/* Close our files */
	fclose(infileBottom);
	fclose(infileTop);
	fclose(outfile);

	return 0;
}
