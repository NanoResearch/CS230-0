
#include "Comparable.hpp"
#include "Printable.hpp"


class ComplexData : public Printable, public Comparable
{
	protected:
		double a;
		double b;
		double r;
		double theta;
	
	public:
		ComplexData(double a, double b);
		virtual ~ComplexData();

		virtual int compareTo(Comparable* c);

		virtual void print();
};

