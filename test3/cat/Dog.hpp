

#include "Pet.hpp"


class Dog : public Pet
{
public:
	Dog(const char* name);
	virtual ~Dog();

	 virtual const char* speak();
};


