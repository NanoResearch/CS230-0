/* process.C
 * This file implements the different prototypes used to process font, meme, and actions files.
 *
 * By: Paul Gerlich
 */

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

#include "process.h"

/* Procsses a full action file */
FILE* loadAct(FILE* infile, struct act* myAct){
	char cur = 'T';
	char* curWord = (char*) calloc(50, sizeof(char));
	int i = 0, fileOut = 0, meme = 0, font = 0, idNum = 0;
	myAct->numTextIDs = getNumIDS(infile);
	myAct->ids = (struct textID*) calloc(myAct->numTextIDs, sizeof(struct textID ) );	
	

	/* Read through the entire file */
	while ( cur != EOF ){
		cur = fgetc(infile);		
			/* Read and process outfile */
		if ( cur == ':' ) {
			if ( strcmp(curWord,"OUTFILE") == 0 ) {
				free(curWord);
				curWord = (char*) calloc(500, sizeof(char));
				goToGrabStringAct(infile, &curWord);
				myAct->outfile = fopen(curWord, "w");
				if ( myAct->outfile == 0 ) {	
					printf("Failed to open file %s.\n", curWord);
					exit(0);
				}
				i = 0;
				free(curWord);
				curWord = (char*) calloc(45, sizeof(char));
				cur = fgetc(infile);
				fileOut = 1;
			} else if ( strcmp(curWord, "MEME") == 0 ) { 	/* Let's grab our meme ID */
				myAct->meme = (char*) calloc(500 , sizeof(char) );
				goToGrabStringAct(infile, &myAct->meme);
				free(curWord);
				curWord = (char*)  calloc(500 , sizeof(char));
				cur = fgetc(infile);
				i = 0;
				meme = 1;
			} else if ( strcmp(curWord, "FONT") == 0 ) { /* Let's grab our font*/
				myAct->font = (char*) calloc(500 , sizeof(char));
				goToGrabStringAct(infile, &myAct->font);
				free(curWord);
				curWord = (char*) calloc(500, sizeof(char));
				cur = fgetc(infile);
				i = 0;	
				font = 1;
			} else {
				myAct->ids[idNum].name = (char*) calloc(500, sizeof(char));
				strcpy(myAct->ids[idNum].name, curWord);

				free(curWord);
				curWord = (char*) calloc(500, sizeof(char));
				fgets(curWord, 400, infile);
				removeNandC(&curWord);
				myAct->ids[idNum].value = (char*) calloc(500, sizeof(char));
				strcpy(myAct->ids[idNum].value, curWord);
				free(curWord);
				curWord = (char*) calloc(500, sizeof(char));
				idNum++;
				cur = fgetc(infile);
				if ( cur == EOF ) {
					break;
				}
				i = 0;
			}
		}


		if ( cur == '\n' || cur == ((char) 13) ) {
			free(curWord);
			curWord = (char*) calloc(500, sizeof(char));
			i = 0;
			cur = fgetc(infile);
		}

		curWord[i] = cur;

		i++;
	}

	if ( !fileOut ) {
		printf("There was no output file tag.\n");
		exit(0);
	} else if ( !font ) {
		printf("No font tag was found.\n");
		exit(0);
	} else if ( !meme ) { 
		printf("No meme tag was found.\n");
		exit(0);
	}	 		

	if ( myAct->numTextIDs == 0 ) {
		printf("There are no text identifiers!\n");
		exit(0);
	}

	free(curWord);

	return myAct->outfile;
}

void processMeme(FILE* infile, struct meme* myMeme, struct act* myAct, struct font* myFont){
	char cur = fgetc(infile);
	char* current = (char*) calloc(50, sizeof(char));
	int i = 0, k = 0, temp = 0;
	int font = 0, meme = 0, memeImage = 0;
	int* numTextIDs = (int*) calloc(myAct->numTextIDs, sizeof(int));
	
	while ( cur != EOF )	{
		current[i] = cur;

		/* Check if action file meme is in meme file */
		if ( strcmp(current,"MEMES") == 0 ) {
			if ( !findString(infile,myAct->meme) ) {
				printf("Could not find designated meme in meme file.\n");
				exit(0);
			}
			free(current);
			current = (char*) calloc(50, sizeof(char));	
			i = -1;
			meme = 1;
		}

		/* Find our font file */
		if ( strcmp(current,"FONTS") == 0 ) {
			free(current);
			findStringOpenFont(infile, myAct->font, myFont);
		

	
			/*
			current = strcat(myAct->font, ".fsf");
			if ( !findString(infile,current) ) {
				printf("Could not find designated font in meme file.\n");
				exit(0);
			}
			myAct->font = (char*) calloc(49, sizeof(char));
			strcpy(myAct->font, current);
			free(current);
			*/
			
			current = (char*) calloc(50, sizeof(char));
			i = -1;
			font = 1;
		}

		/* Check the meme identifiers*/	
		if ( strcmp(current, myAct->meme) == 0 && meme) {
			temp = checkAndStoreID(infile, myAct, myMeme);
			/*
			printf("Happened. Temp = %d\n", temp);
			*/
			if ( temp == 1) {
				numTextIDs[k] = 1;
				k++;		
			} else if ( temp == 2 ) {
				memeImage = 1;
			}

			free(current);
			current = (char*) calloc(50, sizeof(char));
			i = -1;
			temp = 0;	
		}

		if ( cur == '\n' || cur == ((char) 13) ) {
			free(current);
			current = (char*) calloc(50, sizeof(char));
			i = -1;
		}

		cur = fgetc(infile);
		i++;	
	}

	if ( !meme ) {
		printf("Couldn't find MEMES tag.\n");
		exit(0);
	} else if ( !memeImage ) {
		printf("Couldn't find our meme's image file tag!\n");
		exit(0);
	} else if ( !font ) {
		printf("Couldn't find the fonts tag!\n");
		exit(0);
	}	
	
	for ( i = 0; i < myAct->numTextIDs; i ++ ) {
		if ( numTextIDs[i] != 1 ) {
			printf("For some reason we couldn't find the information for all of the text id's specified in the act file\n");
			exit(0);
		}
	}
	
	free(current);
	free(numTextIDs);	
}

/* Used to examine the second variable after our meme. This include file and text ID's */
int checkAndStoreID(FILE* infile, struct act* myAct, struct meme* myMeme){
	/* First will always be a space */
	char cur = fgetc(infile);
	char* current = (char*) calloc(50, sizeof(char));
	int i = 0;

	/* This will be the first actual letter of our text identifier */
	cur = fgetc(infile);
	while ( cur != ':' ) {
		current[i] = cur;
		i++;
		cur = fgetc(infile);
	}

	/* If it's the file, load the file -- assuming file is a protected word. */
	if ( strcmp(current,"FILE") == 0 ) {
		free(current);
		current = (char*) calloc(60, sizeof(char));
		fgets(current, 55, infile);
		removeNandC(&current);
		stringToL(&current);
		myMeme->imageFile = fopen(current, "r");
		if ( myMeme->imageFile == 0 ) {
			printf("Unable to open meme file.\n");
			exit(0);
		}
		myMeme->myImage = process(myMeme->imageFile);
		i = -1;
		free(current);
		return 2;
	}	


	/* See if it matches our ID */
	i = 0;
	while ( i < myAct->numTextIDs ) {
		if ( strcmp(current,myAct->ids[i].name) == 0 ) {
			goToGrabXY(infile, &myAct->ids[i].x, &myAct->ids[i].y);
			free(current);
			return 1;
		}	
	i++;
	}

	return 0;
}


/* Process a font file and it's characters */
void processFont(struct act* myAct, struct font* myFont) {
	char cur;
	int i = 0, index = 0;
	int fontImage = 0, chars = 0;
	char* buffer = (char*) calloc(50, sizeof(char));

	/*
	myFont->infile = fopen(myAct->font, "r");
	if ( myFont->infile == 0 ) {
		printf("Unable to open font file.\n");
		exit(0);
	}
	*/
	
	countNumChars(myFont);	

	myFont->myChars = (struct character*) malloc(sizeof(struct character) * myFont->numChars);	
	
	cur = fgetc(myFont->infile);

	while ( cur != EOF ) {	
		buffer[i] = cur;

		/* Load and process the input simp image */
		if ( strcmp(buffer,"IMAGE") == 0 ) {
			free(buffer);
			buffer = (char*) calloc(500,sizeof(char));
			goToGrabString(myFont->infile, &buffer);
			stringToL(&buffer);
			myFont->imageFile = fopen(buffer, "r");
			if ( myFont->imageFile == 0 ) {
				printf("Failed to open font's image file.\n");
				exit(0);
			}
			myFont->myImage = process(myFont->imageFile);
			free(buffer);
			buffer = (char*) calloc(500,sizeof(char));
			i = -1;	
			fontImage = 1;		
		}
		

		/* Encountered a character */
		if ( strcmp(buffer,"CHARACTER") == 0 ) {	
			processChar(myFont, index);
			index++;
			free(buffer);
			buffer = (char*) calloc(50, sizeof(char));
			i = -1;
			chars = 1;
		}

		if ( cur == '\n' || cur == ((char) 13) ) {
			free(buffer);
			buffer = (char*) calloc(500,sizeof(char));
			i = -1;
		}

		i++;
		cur = fgetc(myFont->infile);
	}

	if ( !fontImage ) {
		printf("There was no font image tag.\n");
		exit(0);
	} else if ( !chars ) {
		printf("There were no characters!\n");
		exit(0);
	}

	free(buffer);
}

/* Counts the total number of characters in the given font */
void countNumChars(struct font* myFont) {
	char cur = fgetc(myFont->infile);
	myFont->numChars = 0;


	while ( cur != EOF ) {
		if (cur == ':') {
			myFont->numChars++;
		}
		cur = fgetc(myFont->infile);
	}

	myFont->numChars -= 2;

	rewind(myFont->infile);
}

 
void freeAct(struct act* myAct) {
	int i = 0;
	free(myAct->meme);
	free(myAct->font);
	for ( i = 0; i < myAct->numTextIDs; i++ ) {
		freeTextID(&myAct->ids[i]);
	}
	free(myAct->ids);
	free(myAct);
}

void freeMeme(struct meme* myMeme) {
	fclose(myMeme->imageFile);
	free(myMeme->myImage.pixels);
	free(myMeme);
}

void freeTextID(struct textID* myID) {
	free(myID->name);
	free(myID->value);
}

void freeFont(struct font* myFont) {
	fclose(myFont->infile);
	fclose(myFont->imageFile);
	free(myFont->myImage.pixels);
	freeChar(myFont->myChars, myFont->numChars);
	free(myFont->myChars);
	free(myFont);
}

void freeChar(struct character* myChar, int numChars) {
	int i;
	for ( i = 0; i < numChars; i++ ) {
		free(myChar[i].thisChar.pixels);
	}
}

void overlayText(FILE* outfile ,struct act* myAct , struct meme* myMeme, struct font* myFont, int i){
	int height = myFont->myChars[0].h;
	int totalWidth = 0;
	double temp = 0;
	int len = strlen(myAct->ids[i].value);
	int* widths = (int*) malloc(len * sizeof(int));
	int topX = 0, topY = 0, centerX = 0, centerY = 0, k = 0;
	struct character* charString = (struct character*) calloc(len, sizeof(struct character));
	
	centerX = myAct->ids[i].x;
	centerY = myAct->ids[i].y;
	
	calculateWidth(myAct, myFont, i, &totalWidth, len, charString, widths);
	
	temp = (double) totalWidth / 2.0 ;
	topX = centerX - temp + 1;
	topY = centerY - height;
	
	for ( k = 0; k < len; k++ ) {
		overlayImages(myMeme->myImage, charString[k].thisChar, topX, topY);
		topX += widths[k];
	}
	
	free(widths);
	free(charString);
}


void calculateWidth(struct act* myAct, struct font* myFont, int i, int* totalWidth, int len, struct character* charString, int* widths){
	int k = 0, j = 0;
	
	for ( k = 0; k < len; k++ ) {
		for ( j = 0; j < myFont->numChars; j++ ) {
			if ( myAct->ids[i].value[k] == myFont->myChars[j].value ) {
				*totalWidth += myFont->myChars[j].w;
				widths[k] = myFont->myChars[j].w;
				charString[k] = myFont->myChars[j];
			}
		}
	}

}



/* Processes the information for the current character */
void processChar(struct font* myFont, int index){
	/* Grab our char value and store it */
	char cur = fgetc(myFont->infile);
	myFont->myChars[index].value = cur;

	goToGrabXYWH(myFont->infile, &myFont->myChars[index].x, &myFont->myChars[index].y, &myFont->myChars[index].w, &myFont->myChars[index].h);

	myFont->myChars[index].thisChar = crop(myFont->myImage, myFont->myChars[index].x, myFont->myChars[index].y, myFont->myChars[index].w, myFont->myChars[index].h);
}



/* Grabs the next string if there are multiple */
int findString(FILE* infile, char* myWord) {	
	char cur = fgetc(infile);
	char* thisLine = (char*) malloc(sizeof(char) * 500);
	char* curWord;
	char* thisPointer;
	
	fgets(thisLine, 500, infile);
	thisPointer = thisLine;	

	curWord = strtok(thisLine, " ");
	while ( curWord != 0) {
		removeNandC(&curWord);	
		/*
		printf("curword: %s\n", curWord);
		*/
		if ( strcmp(curWord,myWord) == 0 ) {
			/* If we can figure out how to properly free curword, all is well and it's 100% perfect! */
			return 1;
		}	
		curWord = strtok(0, " ");
	} 

	return 0;
}

/* Grabs the next string if there are multiple */
int findStringOpenFont(FILE* infile, char* fontName, struct font* myFont) {
	char cur = fgetc(infile);
	char* thisLine = (char*) malloc(sizeof(char) * 50);
	char* compareString = (char*) malloc(sizeof(char) * 50);
	int counter = 0;
	char* curWord;
	char* thisPointer;
	
	fgets(thisLine, 99, infile);
	thisPointer = thisLine;	

	curWord = strtok(thisLine, " ");
	while ( curWord != 0) {
		removeNandC(&curWord);	
		myFont->infile = fopen(curWord, "r");
		
	/*
		printf("curword: %s\n", curWord);
		printf("strlen: %d\n", strlen(curWord));
	*/	
		
		if (myFont->infile == 0 ) {
			printf("Failed when attempting to open a font.\n");
			exit(0);
		}
		
		cur = fgetc(myFont->infile);
		
		while (cur != ':'){
			compareString[counter] = cur;
			counter++;
			cur = fgetc(myFont->infile);
		}
		
		free(compareString);
		compareString = (char*) calloc(50, sizeof(char));
		
		fgets(compareString, 99, myFont->infile);
		removeNandC(&compareString);
		
	/*
		printf("comparestring: %s\n", compareString);
		printf("Font name: %s\n", fontName);
	*/	
		
		if ( strcmp(compareString, fontName) == 0) {
			free(compareString);
			rewind(myFont->infile);
			return 1;
		}
		
		free(compareString);
		compareString = (char*) malloc(sizeof(char) * 50);
		fclose(myFont->infile);
		curWord = strtok(0, " ");
	} 

	free(thisLine);
	free(compareString);
	free(curWord);
	free(thisPointer);
	return 0;
}


void stringToL(char** myWord) {
	int len = 0, i = 0;
	char* buffer = (char*) calloc(45, sizeof(char));	
	strcpy(buffer,*myWord);
	
	len = strlen(*myWord);

	for ( i = 0; i < len; i++) {
		buffer[i] = tolower(buffer[i]);		
	}

	strcpy(*myWord, buffer);
	free(buffer);	
}

/* Will grab the string value listed after a variable */
void goToGrabString(FILE* infile, char** curWord) {
	char cur = fgetc(infile);
	fgets(*curWord, 40, infile);
	removeNandC(curWord);
}

/* Will grab the string value listed after a variable for Act, lazy last-minute implemenations*/
void goToGrabStringAct(FILE* infile, char** curWord) {
	fgets(*curWord, 40, infile);
	removeNandC(curWord);
}

/* Remove newline characters */
void removeNandC(char** curWord){
	int i = 0;
	char* buffer = (char*) calloc(510, sizeof(char));
	strcpy(buffer, *curWord);


	for ( i = 0; i < strlen(buffer); i++ ) {
		if ( buffer[i] == ((char) 13) || buffer[i] == '\n' ) {	
			buffer[i] = 0; 
		}
	}

	strcpy(*curWord, buffer);
	free(buffer);
}


/* Will grab X/Y values (IS there a better way to modularize this?) */
void goToGrabXY(FILE* infile, int* x, int* y) {
	char cur = 'T';
	char* thisLine = (char*) malloc(50 * sizeof(char));

	fgets(thisLine, 50, infile);
	removeNandC(&thisLine);

	*x = atoi(strtok(thisLine, " "));
	*y = atoi(strtok(0, " "));
	
	
	free(thisLine);
}

/* Will grab X/Y/W/H values */
void goToGrabXYWH(FILE* infile, int* x, int* y,int* w, int* h){
	char cur = 'T';
	char* thisLine = (char*) malloc(50 * sizeof(char));

	/* Go to end of variable */
	while ( cur != ':') {
		cur = fgetc(infile);
	}

	fgets(thisLine, 50, infile);
	removeNandC(&thisLine);

	*x = atoi(strtok(thisLine, " "));
	*y = atoi(strtok(0, " "));
	*w = atoi(strtok(0, " "));
	*h = atoi(strtok(0, " "));

	free(thisLine); 
}


/* For the action file, we read through the number of ID's to determine how much to malloc and allows us to grab what we need. */
int getNumIDS(FILE* infile){
	char* num = (char*) calloc(50, sizeof(char));
	char c = 'T';
	int i = 0, numIDS = 0;


	while ( c != EOF ) {
		if ( c == ':' ) {
			numIDS++;
		}
		c = fgetc(infile);		
	}

	numIDS -= 3;

	rewind(infile);	
	free(num);	

	return numIDS;
}


struct image crop (struct image in, int x, int y, int w, int h)
{
	struct image out;
	int i, j, k = 0, thisPixel = 0;

	out.width = w;
	out.height = h;
	out.totalPixels = out.width * out.height;
	
	out.pixels = (struct pixel*) calloc(out.totalPixels, sizeof(struct pixel));


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

struct image overlayImages(struct image bottom, struct image top, int x, int y)
{
	double r, g, b, a;
	int i, j, thisPixel = 0, k = 0;


	/* Iterate through all the pixels and keep which ones we want! */
	for ( i = 0; i < bottom.height; i++ ) {
		for ( j = 0; j < bottom.width; j++) {
			if ( (j >= x && j < (x + top.width) ) && ( i >= y && i < ( y + top.height ) ) ) {
					/* Overlay two images based on Alpha value of top and then calculates resulting pixels RGBA values */
					r = (double) ( ( top.pixels[k].a ) / 255.0 ) * top.pixels[k].r + ( ( ( bottom.pixels[thisPixel].a * ( 255.0 - top.pixels[k].a ) ) / ( 255.0 * 255.0 ) ) * bottom.pixels[thisPixel].r );
					g = (double) ( ( top.pixels[k].a ) / 255.0 ) * top.pixels[k].g + ( ( ( bottom.pixels[thisPixel].a * ( 255.0 - top.pixels[k].a ) ) / ( 255.0 * 255.0 ) ) * bottom.pixels[thisPixel].g );
					b = (double) ( ( top.pixels[k].a ) / 255.0 ) * top.pixels[k].b + ( ( ( bottom.pixels[thisPixel].a * ( 255.0 - top.pixels[k].a ) ) / ( 255.0 * 255.0 ) ) * bottom.pixels[thisPixel].b );
					a = (double) 255.0 * ( ( top.pixels[k].a / 255.0 ) + ( ( bottom.pixels[thisPixel].a * ( 255.0 - top.pixels[k].a ) ) / ( 255.0 * 255.0 ) ) );
					bottom.pixels[thisPixel].r = (unsigned char) r;
					bottom.pixels[thisPixel].g = (unsigned char) g;
					bottom.pixels[thisPixel].b = (unsigned char) b;
					bottom.pixels[thisPixel].a = (unsigned char) a;
					k++; 
			}
			thisPixel++;
		}
	}	

	return bottom;
}
