

target : dependencies
	commands

COMPILE=gcc -ansi -pedantic -g -Og

all : stringcmd

stringcmd : stringstruct.o another.o
	$(COMPILE) -o stringcmd stringcmdmain.c stringstruct.o another.o  

stringstruct.o : stringstruct.c header
	$(COMPILE) -c stringstruct.c 

another.o : another.c
	$(COMPILE) -c another.c

clean :
	rm stringstruct.o another.o stringcmd


