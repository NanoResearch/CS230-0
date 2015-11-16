
#include <stdio.h>
#include "Pet.hpp"


Pet::Pet(const char* name)
{
	this->name = name;
}

Pet::~Pet()
{
	printf("Pet destroyed!\n");
}

const char* Pet::getName()
{
	return this->name;
}


const char* Pet::speak()
{
	return "";
}







