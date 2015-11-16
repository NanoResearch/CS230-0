/* Meme.c
 * This file is used for loading and processing Memes, fonts, and action files to output a meme.
 *
 */


#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<ctype.h>

void debug(){
	printf("I'M SO HACKING!\n");
}

#include "lap.h"
#include "process.c"


void help();

int main (int argc, char** argv) {
	struct act* myAct = (struct act*) malloc(sizeof(struct act) + 1);
	struct meme* myMeme = (struct meme*) malloc(sizeof(struct meme) + 1);
	struct font* myFont = (struct font*) malloc(sizeof(struct font) + 1);
	FILE* memeFile;
	FILE* actFile;
	FILE* outfile;
	int i = 0;
	
	/* Check to make sure they have valid number of arguments */
	if ( argc != 3 ) {
		help();
		exit(0);
	}

	/*Try and open the specified files real quick */
	memeFile = fopen(argv[1], "r");
	if ( memeFile == 0 ) {
		printf("Failed to open %s file.\n", argv[1]);
		exit(0);
	}

	actFile = fopen(argv[2], "r+");
	if ( actFile == 0 ) {
		printf("Failed to open %s file.\n", argv[2]);
		exit(0);
	}


	outfile = loadAct(actFile, myAct);
	processMeme(memeFile, myMeme, myAct, myFont);	
	processFont(myAct, myFont);


	for ( i = 0; i < myAct->numTextIDs; i ++ ) {
		overlayText(outfile, myAct, myMeme, myFont, i);
	}
	
	export(myMeme->myImage, outfile);
	
	fclose(memeFile);
	fclose(actFile);
	fclose(outfile);
	
	freeAct(myAct);
	freeMeme(myMeme);
	freeFont(myFont);

	return 0;
}


void help(){
	printf("----------------------------------------------------\n");
	printf("For help understanding this program, type cat README\n");
	printf("----------------------------------------------------\n");
}

