

#ifndef PET_HPP__
#define PET_HPP__



class Pet
{
private:
	const char* name;


public:
	Pet(const char* name);
	virtual ~Pet();

	const char* getName();

	/*  virtual is important! */
	const char* speak();

};





#endif



