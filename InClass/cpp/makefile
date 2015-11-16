# Makefile for stringtest executable

all:stringtest

stringtest: stringtest.cpp String.o String.hpp
	g++ -Wall -g -o stringtest stringtest.cpp String.o

String.o: String.cpp String.hpp
	g++ -Wall -g -c String.cpp
	
clean:
	rm *.o stringtest