
#include "Comparable.hpp"
#include "Sort.hpp"


void sort(Comparable** list, int N)
{
	for ( int i = 0; i < N; i++ )
	{
		for ( int j = i+1; j < N; j++ )
		{
			if ( list[i]->compareTo(list[j]) > 0 )
			{
				Comparable* t = list[i];
				list[i] = list[j];
				list[j] = t;
			}
		}
	}
}
