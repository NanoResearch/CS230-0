/**
 * String.hpp 
 *
 * Contains a class for dealing with strings.
 */

/** TODO: Add Header Guards */

#ifndef _HEADER_
#define _HEADER_

class String
{
private:


protected:
	char* str;
	int length; // length of the c-string
	int size; // number of chars in str

public:
	String(); // Default constructor
	String(const char* s); // Constructor with a given string
	String(const String& s); // Copy Constructor
	

	virtual ~String(); // Destructor

	int getLength();

	const char* getCString();

	void concatenate(const char* s);
	void concatenate(String* s);

	String* copy(); // Get me a copy of this String

	int indexOf(char c); // get the index of the first instance of c
	int indexOf(char c, int i); // the the index of c after index i

	/**
	 * Overload the [] operator to get the ith character.
	 */
	char get(int i);

	char operator[](int i);	

	void operator+=(const char* s);



}; // THIS SEMICOLON IS IMPORTANT!



String operator+(String& a,String& b);
#endif



