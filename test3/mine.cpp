#include "mine.hpp"
#include <stdio.h>

int main(){
	int i = 5;
	mein<int>* mine = new mein<int>(5);


	printf("%d\n", mine->get());

	return 0;

}
