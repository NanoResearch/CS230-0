


all : ssbros


objects : 
	g++ -g -c GameManager.cpp
	g++ -g -c Pursuer.cpp
	g++ -g -c OtherActors.cpp
	g++ -g -c smarthero.cpp
	g++ -g -c smartpowerup.cpp
	g++ -g -c smartenemy.cpp
	g++ -g -c GraphMap.cpp
	g++ -g -c Actor.cpp

ssbros : ssbros.cpp OtherActors.o smarthero.o smartpowerup.o smartenemy.o
	g++ -g -Wall -o ssbros ssbros.cpp Actor.o GraphMap.o GameManager.o OtherActors.o smarthero.o smartpowerup.o smartenemy.o Pursuer.o -lncurses

OtherActors.o: OtherActors.cpp
	g++ -g -Wall -c OtherActors.cpp
	
smarthero.o: smarthero.cpp
	g++ -g -Wall -c smarthero.cpp
	
smartpowerup.o: smartpowerup.cpp
	g++ -g -Wall -c smartpowerup.cpp
	
smartenemy.o : smartenemy.cpp
	g++ -g -Wall -c smartenemy.cpp

# Modify the clean target to remove YOUR .o files...
clean:
	rm ssbros OtherActors.o smarthero.o smartpowerup.o smartenemy.o

tarball:
	tar czf 612500889.tar.gz Makefile README *.hpp *.cpp *.o maps/*.txt

