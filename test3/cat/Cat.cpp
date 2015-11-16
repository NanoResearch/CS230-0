#include "Cat.hpp"
#include <stdio.h>

// Cheshire::Cheshire(const char* name) : Cat(name)
Cat::Cat(const char* name) : Pet(name)
{
	this->num_lives = 9;
}	




Cat::~Cat()
{
	printf("Cat destroyed!\n");
}

const char* Cat::speak()
{
	return "Destroy all humans!";
}

int Cat::getNumLives()
{
	return this->num_lives;
}

void Cat::die()
{
	this->num_lives = this->num_lives - 1;
}
