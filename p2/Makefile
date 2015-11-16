


all : ssbros


objects : 
	g++ -g -c GameManager.cpp
	g++ -g -c Pursuer.cpp
	g++ -g -c OtherActors.cpp
	g++ -g -c simplehero.cpp
	g++ -g -c GraphMap.cpp
	g++ -g -c Actor.cpp

ssbros : ssbros.cpp OtherActors.o simplehero.o
	g++ -g -Wall -o ssbros ssbros.cpp Actor.o GraphMap.o GameManager.o OtherActors.o simplehero.o Pursuer.o -lncurses

OtherActors.o: OtherActors.cpp
	g++ -g -Wall -c OtherActors.cpp
	
simplehero.o: simplehero.cpp
	g++ -g -Wall -c simplehero.cpp

# Modify the clean target to remove YOUR .o files...
clean:
	rm ssbros OtherActors.o simplehero.o

tarball:
	tar czf 612500889.tar.gz Makefile README *.hpp *.cpp *.o maps/*.txt

