 
#include <stdio.h>

#include "Dog.hpp"

Dog::Dog(const char* name) : Pet(name) 
{
	
}

Dog::~Dog()
{
	printf("Dog destroyed!\n");
}

const char* Dog::speak()
{
	return "Bark!";
}


