


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

	


test: testsimplehero testsmarthero testsmartenemy testsmartpowerup

testsimplehero: ssbros testsimplehero_easy testsimplehero_medium testsimplehero_hard 


testsimplehero_easy: testsimplehero_basic testsimplehero_open testsimplehero_twisteasy testsimplehero_20x20 testsimplehero_open_memory testsimplehero_basic_memory
	@-echo "End Test Simplehero EASY (75 pts total)"

testsimplehero_basic:
	-./ssbros maps/basic.txt --hero simplehero --delay-enemy 4 --delay-powerup 2 --test TEST_SIMPLEHERO_BASIC --goal hero --moves 100 --delay 100 --runpts 4 --goalpts 11 --time 5

testsimplehero_open:
	-./ssbros maps/open.txt --hero simplehero --delay-enemy 4 --delay-powerup 2 --test TEST_SIMPLEHERO_OPEN --moves 100 --delay 100 --runpts 4 --goalpts 11 --time 5

testsimplehero_twisteasy:
	-./ssbros maps/twisteasy.txt --hero simplehero --delay-powerup 4 --test TEST_SIMPLEHERO_TWIST_EASTY --moves 10000 --delay 0 --runpts 4 --goalpts 11 --time 250

testsimplehero_20x20:
	-./ssbros maps/random20x20.txt --hero simplehero --delay-powerup 4 --test TEST_SIMPLEHERO_20x20 --moves 1000 --delay 0 --runpts 4 --goalpts 11 --time 30

testmemory: testsimplehero_open_memory testsimplehero_basic_memory

testsimplehero_open_memory:
	@-echo "Simplehero EASY memory test (open): 10 points, -2 for each memory warning or leak"
	-valgrind --leak-check=full --dsymutil=yes ./ssbros maps/open.txt --hero simplehero --test TEST_SIMPLEHERO_OPEN_MEMORY --render-off
	@-echo "Were there leaks? -2/10 for each!"
	@-echo

testsimplehero_basic_memory:
	@-echo "Simplehero EASY memory test (basic): 5 points, -1 for each memory warning or leak"
	-valgrind --leak-check=full --dsymutil=yes ./ssbros maps/basic.txt --hero simplehero --test TEST_SIMPLEHERO_BASIC_MEMORY --render-off
	@-echo "Were there leaks? -1/5 for each!"
	@-echo

testsimplehero_medium: testsimplehero_cycle testsimplehero_tree testsimplehero_4waypossible testsimplehero_pacman testsimplehero_50x50 testsimplehero_75x25 testsimplehero_100x20 testsimplehero_twistmed
	@-echo "End Test Simplehero MEDIUM (60 pts total)"

testsimplehero_cycle:
	-./ssbros maps/cycle.txt --hero simplehero --delay-enemy 4 --delay-powerup 2 --test TEST_SIMPLEHERO_CYCLE --moves 500 --delay 10 --runpts 1 --goalpts 7 --time 100

testsimplehero_tree:
	-./ssbros maps/tree.txt --hero simplehero --test TEST_SIMPLEHERO_TREE --moves 500 --delay 1000 --runpts 1 --goalpts 7 --time 300

testsimplehero_4waypossible:
	-./ssbros maps/4waypossible.txt --hero simplehero --test TEST_SIMPLEHERO_4WAY_POSSIBLE --moves 1000 --delay 0 --runpts 1 --goalpts 7 --time 400

testsimplehero_pacman:
	-./ssbros maps/pacman.txt --hero simplehero --disable-enemies --test TEST_SIMPLEHERO_PACMAN --moves 1000 --delay 0 --delay-powerups 2 --runpts 1 --goalpts 7 --time 300

testsimplehero_50x50:
	-./ssbros maps/random50x50.txt --hero simplehero --disable-enemies --test TEST_SIMPLEHERO_50x50 --moves 20000 -delay 0 --delay-powerups 2 --runpts 1 --goalpts 7 --time 1500

testsimplehero_75x25:
	-./ssbros maps/random75x25.txt --hero simplehero --disable-enemies --test TEST_SIMPLEHERO_75x25 --moves 20000 --delay 0 --delay-powerups 2 --runpts 1 --goalpts 7 --time 1500

testsimplehero_100x20:
	-./ssbros maps/random100x20.txt --hero simplehero --disable-enemies --test TEST_SIMPLEHERO_100x20 --moves 2000 --delay 0 --delay-powerups 2 --runpts 0 --goalpts 6 --time 1500

testsimplehero_twistmed:
	-./ssbros maps/twistmed.txt --hero simplehero --disable-enemies --test TEST_SIMPLEHERO_TWIST_MED --moves 2000 --delay 0 --delay-powerups 2 --runpts 0 --goalpts 6 --time 1200





testsimplehero_hard: testsimplehero_sc1 testsimplehero_sc2 testsimplehero_sc3 testsimplehero_plinko
	@-echo "End Test Simplehero HARD (15pts total)"

testsimplehero_sc1:
	-./ssbros maps/stronglyconnected.txt --hero simplehero --delay-enemy 4 --delay-powerup 2 --test TEST_SIMPLEHERO_STRONGLYCONNECTED1 --delay 100 --runpts 0 --goalpts 4 --time 200

testsimplehero_sc2:
	-./ssbros maps/stronglyconnected2.txt --hero simplehero --delay-enemy 4 --delay-powerup 2 --test TEST_SIMPLEHERO_STRONGLYCONNECTED2 --delay 100 --runpts 0 --goalpts 3 --time 200

testsimplehero_sc3:
	-./ssbros maps/stronglyconnected3.txt --hero simplehero --delay-enemy 4 --delay-powerup 2 --test TEST_SIMPLEHERO_STRONGLYCONNECTED3 --delay 100 --runpts 0 --goalpts 3 --time 200

testsimplehero_plinko:
	-./ssbros maps/plinko.txt --hero simplehero --test TEST_SIMPLEHERO_PLINKO --delay 0 --runpts 0 --goalpts 5 --delay-powerup 10 --delay 100 --time 500

testsmarthero: ssbros 


testsmartenemy: ssbros


testsmartpowerup: ssbros


