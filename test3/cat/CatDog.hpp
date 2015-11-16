#include "Cat.hpp"
#include "Dog.hpp"

class CatDog : public Cat, Dog
{
	public:
	CatDog(const char* name);
	virtual ~CatDog();

};
