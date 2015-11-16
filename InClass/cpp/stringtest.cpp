#include <stdio.h>
#include "String.hpp"

int main(int argc, char** argv)
{
	String* s = new String(argv[0]);

	String a = *s; // Make a copy of s
	
	for ( int i = 1; i < argc; i++ )
	{
		//s->concatenate(" ");
		// s->concatenate(argv[i]);
		*s += " ";
		*s += argv[i];

		a += " ";
		a += argv[i];
	}

	printf("%s\n", s->getCString());
	printf("%s\n", a.getCString());

	for ( int i = 0; i < s->getLength(); i++ )
	{
		printf("%c", s->get(i));
	}
	printf("\n");

	for ( int i = 0; i < a.getLength(); i++ )
	{
		printf("%c", a[i]);
	}
	printf("\n");

	String t = a + " ";
	t = t + *s;
	printf("%s\n", t.getCString());

	delete s;
	s = 0;
	
	return 0;
}

