

#include "Pet.hpp"

class Cat : public Pet
{
protected:
	int num_lives;
	
public:
	Cat(const char* name);
	virtual ~Cat();

	virtual const char* speak();

	int getNumLives();

	void die();
};


