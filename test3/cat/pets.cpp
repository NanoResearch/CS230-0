 

#include <stdio.h>
#include "Pet.hpp"
#include "CatDog.hpp"


int main(void)
{
	Pet* p = new Pet("Iggy the Iguana");
	Pet* d = new Dog("Cici");
	Pet* c = new Cat("Scratch Fury, Destroyer of Worlds");
	Dog* dog = (Dog*)d;
	Pet* cd = new CatDog("WHAT?");


	printf("%s says, \"%s\"\n", p->getName(), p->speak());
	printf("%s says, \"%s\"\n", d->getName(), d->speak());
	printf("%s says, \"%s\"\n", cd->getName(),cd->speak());
	printf("%s says, \"%s\"\n", c->getName(), c->speak());

	delete p; p = 0;
	delete d; d = 0; dog = 0; 
	delete c; c = 0;
	delete cd;


	return 0;
}
