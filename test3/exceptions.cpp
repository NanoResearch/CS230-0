#include <iostream>
#include <stdio.h>
#include <string.h>

int main(void)
{
	printf("Enter a number:\n");
	try
	{
		// DO SOMETHING!
		int i;
		char error[50];

		strcpy(error, "Some Shit");

		std::cin >> i;

		// something may be thrown!
	//
		throw (char*) "wit";
		//
	}
/*
	catch ( int error_code )
	{
		printf("Error: %d\n", error_code);
	}
*/
	catch ( char* error_msg )
	{
		printf("got error: %s\n", error_msg);		
		throw;
	}
	catch ( std::exception e )
	{
		printf("%s\n", e.what() );
	}

	catch (...)
	{
		printf("Unknown exception type!\n");
		//throw;
	}

	return 0;
}
