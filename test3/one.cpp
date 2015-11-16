#include <stdio.h>
#include "one.hpp"

myClass::myClass(){
	this->i = 10;
}

myClass::~myClass(){
printf("Destroyed!\n");
}

int myClass::get(){
	return this->i;
}





int main(){
	myClass* mine = new myClass();

	printf("my i: %d\n", mine->get());


	delete mine;
}
