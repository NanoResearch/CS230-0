#include <stdio.h>
#include <string.h>


int main(int argc, char** argv)
{
	if ( argc < 3 )
	{
		printf("Usage: compare file1 file2\n");
		return 1;
	}


	FILE* f1 = fopen(argv[1], "rb");
	FILE* f2 = fopen(argv[2], "rb");

	if ( !f1 )
	{
		printf("%s failed to open!\n", argv[1]);
		return 0;
	}
	if ( !f2 )
	{
		printf("%s failed to open!\n", argv[2]);
		return 0;
	}
	int total_bytes = 0;
	int bytes_off = 0;
	unsigned char c1, c2;
	while ( !feof(f1) && !feof(f2) )
	{
		c1 = fgetc(f1);
		c2 = fgetc(f2);

		if ( (int)c1 - (int)c2 > 2 || (int)c2 - (int)c1 > 2 )
		{
			bytes_off++;
		}

		total_bytes ++;
	
	}

	int same_len = 1;
	if ( feof(f1) != feof(f2) )
	{
		same_len = 0;
		printf("Files are not the same length!\n");
	}

	fclose(f1);
	fclose(f2);

	printf("%d bytes of %d significantly different.\n", bytes_off, total_bytes);

	if ( argc == 4)
	{
		printf("%d points.\n",(int)( atoi(argv[3]) * (1.0 - (((double)bytes_off)/(double)total_bytes))));
	}
	else if ( argc > 4 )
	{	
		int percent = (int)( atoi(argv[3]) * (1.0 - (((double)bytes_off)/(double)total_bytes)));
		int fsize = atoi(argv[4])*same_len;
		printf("%2d = %2d + %2d points.\n", percent+fsize, fsize, percent);
	}
	
	return 0;
}
