

#include "String.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* to implement a class method... */
/* returntype ClassName::functionName(params) {   } */

int String::getLength()
{
	return this->length;
}

char String::get(int i)
{
	if ( i < 0 || i >= this->size )
	{
		return -1;
	}

	/* "this" is a resevered word */
	return this->str[i];
}

String::String()
{
	// Default constructor
	this->size = 100;
	this->length = 0;

	this->str = (char*)malloc(this->size);

	bzero(this->str, this->size);
}

String::String(const char* str)
{
	if ( str != 0 )
	{

		this->length = strlen(str);
		this->size = this->length + 1;
	
		this->str = (char*)malloc(this->size);
		strcpy(this->str, str); // destination, source
	}
	else
	{
		/* HERE IS THE PROBLEM... */
		// String::String();
		//
		// TODO: copy the default constructor here!
	}
}

String::String(const String& s)
{
	this->length = s.length;
	this->size = s.size;
	// this->str = s.str; // We want a deep copy!

	this->str = (char*)malloc(this->size);

	strcpy(this->str, s.str);
}








String::~String()
{
	printf("Destructor Called!\n");
	
	if ( this->str != 0 )
	{
		free(this->str);
		this->str = 0;
	}
	
	this->size = 0;
	this->length = 0;
}

const char* String::getCString()
{
	return this->str;
}


void String::concatenate(const char* s)
{
	int l = strlen(s);

	if ( this->size <= this->length + l + 1 )
	{
		/* Realloc */
		this->size = this->length + l + 10;
		this->str = (char*)realloc(this->str, this->size);
	}

	strcat( this->str, s);
	this->length = this->length + l;
}


char String::operator[](int i)
{
	return this->get(i);
}

void String::operator+=(const char* s)
{
	this->concatenate(s);
}


String operator+( String& a, String& b)
{
	String c;
	c += a.getCString();
	c += b.getCString();
	return c;
}












