#include <stdio.h>
#include <math.h>
#include "ComplexData.hpp"


#define PI 3.14159265

ComplexData::ComplexData(double a, double b)
{
	this->a = a;
	this->b = b;
	this->r = sqrt(a*a + b*b);
	this->theta = atan( b / a );

	if ( a < 0 )
	{
		this->theta = this->theta + PI;
	}
}


ComplexData::~ComplexData()
{
	printf("Destryoing ");
	this->print();
}

int ComplexData::compareTo(Comparable* c)
{
	ComplexData* d = (ComplexData*)c;

	if ( this->r < d->r )
	{
		return -2;
	}
	if ( this->r > d->r )
	{
		return 2;
	}

	// Same radius, different angles!
	if ( this->theta < d->theta )
	{
		return -1;
	}

	if ( this->theta > d->theta )
	{
		return 1;
	}

	// Equal!
	return 0;
}


void ComplexData::print()
{
	printf("%lf + %lfi = %lfe^{%lfi}\n", this->a, this->b, this->r, this->theta);
}
