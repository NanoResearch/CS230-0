#ifndef _PROCESS_
#define _PROCESS_

#include "lap.h"

/* Struct for storing charactesr */
struct character {
	struct image thisChar;
	char value;
	int x;
	int y;
	int w;
	int h;
};

/* Struct for storing an entire font */
struct font {
	FILE* infile;	
	FILE* imageFile;
	struct image myImage;
	struct character* myChars;
	int numChars;
};

/* Struct for storing a textID */
struct textID {
	char* name;
	char* value;
	int x;
	int y;
};

/* Struct for storing a meme */
struct meme {
	FILE* imageFile;
	struct image myImage;
};

/* Struct for storing an Action */
struct act {
	FILE* outfile;
	char* meme;
	char* font;
	struct textID* ids;
	int numTextIDs;
};

/* Declare some prototype functions */
struct font loadFont(FILE* infile);
struct image crop (struct image in, int x, int y, int w, int h);
struct image overlayImages(struct image bottom, struct image top, int x, int y);
FILE* loadActFile(FILE* infile, struct act* myAct);
void goToGrabString(FILE* infile, char** curWord);
void goToGrabStringAct(FILE* infile, char** curWord);
void goToGrabXY(FILE* infile, int* x, int* y);
void goToGrabXYWH(FILE* infile, int* x,int* y,int* w, int* h);
void grabNextInt(FILE*infile, int* myInt);
void collectTextIDs(struct act* myAct, FILE* infile);
void processMeme(FILE* infile, struct meme* myMeme, struct act* myAct, struct font* myFont);
void processFont(struct act* myAct, struct font* myFont);
void removeNandC(char** curWord);
void stringToL(char** curWord);
void processChar(struct font* myFont, int index);
void countNumChars(struct font* myFont);
void overlayText(FILE* outfile, struct act* myAct, struct meme* myMeme, struct font* myFont, int i);
void calculateWidth(struct act* myAct, struct font* myFont, int i, int* width, int len, struct character* myChars, int* widths);
void freeAct(struct act* myAct);
void freeMeme(struct meme* myMeme);
void freeFont(struct font* myFont);
void freeTextID(struct textID* myText);
void freeChar(struct character* myChar, int numChars);
int checkAndStoreID(FILE* infile, struct act* myAct, struct meme* myMeme);
int findString(FILE* infile, char* myWord);
int getNumIDS(FILE* infile);

#endif
