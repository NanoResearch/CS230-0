
all: complex

complex: complex.cpp ComplexData.o Sort.o
	g++ -g -Wall -o complex complex.cpp ComplexData.o Sort.o

ComplexData.o: ComplexData.cpp ComplexData.hpp Comparable.hpp Printable.hpp
	g++ -g -Wall -c ComplexData.cpp

Sort.o: Comparable.hpp Sort.hpp Sort.cpp
	g++ -g -Wall -c Sort.cpp

clean:
	rm *.o complex
