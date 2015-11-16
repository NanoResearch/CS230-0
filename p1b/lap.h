/* lap.h: Load and Process header file
 * 
 * Despite the apparent fact that this is bad practice, this header file will be acting as
 * a library (containing full methods) to be used with any program that needs to load
 *  in and process/store/export the image data from a SIMP file.
 *
 * By: Paul Gerlich
 */


#ifndef _LAPHEADER_
#define _LAPHEADER_
struct pixel{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

struct image{
	int width;
	int height;
	int totalPixels;
	struct pixel* pixels;
};


/* Function for processing a binary SIMP file and collecting Height, Width, and pixels  */
struct image process( FILE* input )
{
	struct image inputImage;
	int i;

	rewind(input);	

	/* Start reading the integers from the binary file. */ 
	fread( &inputImage.width, 4, 1, input );	
	fread( &inputImage.height, 4, 1, input );
	
		
	if (inputImage.width <= 0) {
		printf("There cannot be a negative width value. File is corrupt.\n");
		exit(1);
	}
	if (inputImage.height <= 0) {
		printf("There cannot be a negative height value. File is corrupt.\n");
		exit(1);
	}

	inputImage.totalPixels = inputImage.width * inputImage.height;

	inputImage.pixels = (struct pixel*)  malloc(inputImage.totalPixels * sizeof(struct pixel));

	/* Collet all the RGBA data for each pixel and store it in a pixel struct array */
	for (i = 0; i < inputImage.totalPixels; i++){
		if ( feof( input ) ){
			printf("Must of hit the EOF early.. File is corrupt.\n");
			exit(1);
		}
		
		fread(&inputImage.pixels[i].r, 1, 1, input);
		fread(&inputImage.pixels[i].g, 1, 1, input);
		fread(&inputImage.pixels[i].b, 1, 1, input);
		fread(&inputImage.pixels[i].a, 1, 1, input);		
	}	

	return inputImage;
}


void export(struct image inputImage, FILE* outfile) {
	int i;
	
	fwrite(&inputImage.width, 4, 1, outfile);
	fwrite(&inputImage.height, 4, 1, outfile);

	for ( i = 0; i < inputImage.totalPixels; i++ ){
		fwrite(&inputImage.pixels[i].r, 1, 1, outfile);
		fwrite(&inputImage.pixels[i].g, 1, 1, outfile);
		fwrite(&inputImage.pixels[i].b, 1, 1, outfile);
		fwrite(&inputImage.pixels[i].a, 1, 1, outfile);
	}

}
#endif
