/*
 * SimpleHero.cpp
 * 	Author: Paul Gerlich
 */
 
 #ifndef _SIMPLEHEROCPP_
 #define _SIMPLEHEROCPP_

#define ISEATABLE 4
#define ISPOWERUP 8
 
 
#include "simplehero.hpp"
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <queue>

SimpleHero::SimpleHero( int type ) : Actor(type)
{
	this->numEatibles = 0;
	this->numPowerups = 0;
	this->maxEReachable = 0;
	this->maxPReachable = 0;
	this->firstRun = 1;
	this->currentTarget = -1;
	this->inPursuit = 0;
	this->stage = 1;
	this->shouldWait = 0;
	this->isSegregated = 0;
	
	this->myPath = new std::stack< std::pair<int, int> >();
}

//Simple destructor
SimpleHero::~SimpleHero()
{
	//printf("started\n");
	// Free eatible array
	for (int i = 0; i < this->numEatibles; i++) {
		free(this->eatibleInfo[i]);
	}
	
	//free(this->eatibleInfo);
	
	this->eatibleInfo = 0;
	
	//Free power up array
	for (int i = 0; i < this->numPowerups; i++){
		free(this->powerupInfo[i]);
	}
	
	//free(this->powerupInfo);
	
	this->powerupInfo = 0;
	
	delete this->myPath;
	
	this->myPath = 0;

	//printf("to destruct\n");
}

/**
This will run based on this basic idealogy:

	So basically we'll check in this order.
	1) Eatibles that of which you can access all other items from
	2) Power ups that of which you can access all other items from
	
	Based on these basics, we decide which block to go to next.
	
	FIXME: Why is there a segfault when I free eatibleInfo? Why does it cause leaks if I don't what to do??
	
	FIXME: Seg faults with powerups


	FIXME: Establish effective communication with mutliple heros/enemies/objects. Make it so that only one person is making the decisions and distributing pointers to paths to other actors so that everything is less computationally expensive. This could be done by assigning the first actor of our desired type in the actor array to the main "brain" actor. Call the methods of this actor and assign based on number of actors??? Check if dead first, if he's dead, move to the next actor that's alive and assign him as the superComputer actor.

	FIXME: implement enemy avoidance strategy (If enemey is within 2 squares, do nothing but note position. Obviouslly if we're about to collide with an enemy go the opposite direction. if enemy is within 1 square and moved toward us and could collide with us, move away from the enemy for one turn and keep moving forward??)
	
*/
int SimpleHero::selectNeighbor( GraphMap* map, int x, int y )
{	
	std::pair <int,int> myLocation;
	int myMove = 0;
	
	myLocation.first = x;
	myLocation.second = y;

	// We want to set up our hero and assess the situation of the entire map before deciding to make any moves.
	if (this->firstRun == 1) {
	
		//Count the number of eatibles and powerups
		this->numEatibles = this->getNumEatibles(map);
		this->numPowerups = this->getNumPowerups(map);
		
		
		//Set max reachable powerup/eatible values. This may be inaccurate at first for segregated maps.
		this->maxEReachable = this->numEatibles;
		this->maxPReachable = this->numPowerups;

		//Make our information arrays
		this->initializeEatible(this->numEatibles);
		this->initializePowerup(this->numPowerups);
		
		//Locate the eatibles
		this->locateEatibles(map);
		this->locatePowerups(map);
		
		//Don't do this firstrun stuff again
		this->firstRun = 0;
		
		//Update how many powerups/eatibles are reachable from each eatible and powerup. Should only be called once!
		this->updateReachable(map); 
		
		//Update the actual powerups/eatibles reachable for this char (in case of segregated map)
		assessReachable(map, myLocation, -1, -1);
		
		if ( this->maxPReachable != this->numPowerups || this->maxEReachable != this->numEatibles ) {
			//printf("We're segregated!\n");
			this->isSegregated = 1;
		}
		
		//printf("This Pacman can reach: %d powerups and %d eatibles\n", this->maxPReachable, this->maxEReachable);
		
		//If for some reason there are no eatibles on the map, we'll go ahead and move on to powerups
		if ( this->numEatibles == 0 ) {
			stage = 2;
		}
	}
	
	//Updates the powerups/eatibles if we are on the location of one (i.e, we just ate one)
	
	this->updateTargets(map, myLocation);
	
	//Make sure we still incorporate check for enemies in stage 2
	if ( !(this->inPursuit)  || ( this->stage == 2 ) ) {
		//printf("Grabbing target\n");
		this->getNextTarget(map, myLocation);
		//printf("Grabbed\n");
		this->inPursuit = 1;
	} else {
		this->checkForEnemies(map, myLocation);
	}
	
	this->chooseMove(map, myMove, myLocation);
	
	return myMove;
}

// Return a new copy of our hero
Actor* SimpleHero::duplicate()
{
	return new SimpleHero(this->getType());
}

// Return Net ID
const char* SimpleHero::getNetId()
{
	return "pgerlich";
}

//Return Actor ID
const char* SimpleHero::getActorId()
{
	return "simplehero";
}

// Counts all of the eatibles on the map
int SimpleHero::getNumEatibles(GraphMap* map)
{
	int numSpots = map->getNumVertices();
	int eatibles = 0;
	
	for (int i = 0; i < numSpots; i++) {
		if ( (map->getActorType(i) & ACTOR_EATABLE) && !(map->getActorType(i) & ACTOR_POWERUP) ) {
			eatibles++;
		}
	}
	
	return eatibles;
}


void SimpleHero::updateReachableNoSearch(int powerup, int eatible){
 
 
	 //Check through all powerups
	 for (int i = 0; i < this->numPowerups; i++ ) {
		//If the powerups still on the map
		if ( this->powerupInfo[i][5] == 1 ) {
			//Remove the eatible/powerup that was just eaten (would only be removed if this one could reach all others! (I think??) )
			if ( this->powerupInfo[i][6] == 1 ) {
				this->powerupInfo[i][3] -= eatible;
				this->powerupInfo[i][4] -= powerup;
			}
			
			//Now assess if it can reach all other eatibles.
			if ( this->powerupInfo[i][3] == this->maxEReachable && this->powerupInfo[i][4] == this->maxPReachable ) {
				//Can now reach all other eatibles;
				this->powerupInfo[i][6] = 1;
			}
		}	
	 }
	 
	 //check through all eatibles
	 for (int i = 0; i < this->numEatibles; i++ ) {
		//If the eatible is still on the map
		if ( this->eatibleInfo[i][5] == 1 ) {
			//Remove the eatible or powerup that was just eaten.
			
			if ( this->eatibleInfo[i][6] == 1 ) {
				this->eatibleInfo[i][3] -= eatible;
				this->eatibleInfo[i][4] -= powerup;
			}
			
			//If it can now reach all eatibles. This is for strongly connected maps (once all other eatibles are eaten, now they can reach all!)
			if ( this->eatibleInfo[i][3] == this->maxEReachable && this->eatibleInfo[i][4] == this->maxPReachable ) {
				//Can now reach all other eatibls
				this->eatibleInfo[i][6] = 1;
			}
		}
	 }

}

// Locates Vertice and X/Y coordinates of eatibles
void SimpleHero::locateEatibles(GraphMap* map)
{
	int numSpots = map->getNumVertices();
	int actor;
	int eatCount = 0;
	int x, y;
	
	for (int i = 0; i < numSpots; i++) {
		actor = map->getActorType(i);
		if ( (actor & ACTOR_EATABLE) && !(actor & ACTOR_POWERUP) ) {
			map->getActorPosition( i, x, y );
			this->eatibleInfo[eatCount][0] = x;
			this->eatibleInfo[eatCount][1] = y;
			this->eatibleInfo[eatCount][2] = map->getVertex(x,y);
			this->eatibleInfo[eatCount][5] = 1;
			eatCount++;
		}
	}
	
}

/** Initialize our Eatible array. Just doing this to modularize code

	Idea of the eatible array:
	
		Per each index:
	[X coordinate] [Y coordinate] [ Vertice # ] [ # of eatibles reachable from this powerup] [ # of powerups reachable from this powerup] [ still available] [ can reach all other target items ]

*/
void SimpleHero::initializeEatible(int numEatibles)
{
	this->eatibleInfo = (int**) malloc(numEatibles * sizeof(int*));
	
	for (int i = 0; i < numEatibles; i++) {
		this->eatibleInfo[i] = (int*) malloc(7 * sizeof(int));
	}
	
}

/** 
Counts all of the power ups on the map
*/
int SimpleHero::getNumPowerups(GraphMap* map)
{
	int numSpots = map->getNumVertices();
	int numPowerups = 0;
	
	for (int i = 0; i < numSpots; i++) {
		if (map->getActorType(i) & ACTOR_POWERUP) {
			numPowerups++;
		}
	}
	
	return numPowerups;
}

/** 
Locates Vertice and X/Y coordinates of eatibles
*/
void SimpleHero::locatePowerups(GraphMap* map)
{
	int numSpots = map->getNumVertices();
	int powerCount = 0;
	int x, y;
	
	for (int i = 0; i < numSpots; i++) {
		//If we're on a powerup
		if (map->getActorType(i) & ACTOR_POWERUP) {
			map->getActorPosition( i, x, y );
			//Update X/Y/Vertex location of all powersup.
			this->powerupInfo[powerCount][0] = x;
			this->powerupInfo[powerCount][1] = y;
			this->powerupInfo[powerCount][2] = map->getVertex(x,y);
			
			//This is a dead powerup!
			if ( x == -1 && y == -1 ) {
				this->powerupInfo[powerCount][5] = 0;
			} else {
				this->powerupInfo[powerCount][5] = 1;
			}
			
			//Increment powerups left.
			powerCount++;
		}
	}
	
}

/**
Simple chooses our next move from the stack.
*/
void SimpleHero::chooseMove(GraphMap* map, int &myMove, std::pair<int,int> myLocation) {

	if ( this->isSegregated && this->maxPReachable == 0 && this->maxEReachable == 0 ) {
		myMove = 0;
		return;
	}
	
	std::pair<int,int> tempCoord = this->myPath->top();
	int myX, myY, nextX, nextY;
	
	myX = tempCoord.first;
	myY = tempCoord.second;
	
	
	for ( int i = 0; i < map->getNumNeighbors(myLocation.first, myLocation.second); i++ ) {
		map->getNeighbor(myLocation.first, myLocation.second, i, nextX, nextY);
		if ( this->shouldWait ) {
			myMove = i;
			this->shouldWait = 0;
			break;
		} else if ( myX == nextX && myY == nextY ) {
			myPath->pop();
			myMove = i;
		}
	}
	
}

/** Initialize our powerup array. Just to modularize code.

	Idea of the powerup array:
		Per each index:
	[X coordinate] [Y coordinate] [ Vertice # ] [ # of eatibles reachable from this powerup] [ # of powerups reachable from this powerup] [ stillAvailable ] [ can reach all other target items ]


*/
void SimpleHero::initializePowerup(int numPowerups)
{
	this->powerupInfo = (int**) malloc(numPowerups * sizeof(int*));
	
	for (int i = 0; i < numPowerups; i++) {
		this->powerupInfo[i] = (int*) malloc(7 * sizeof(int));
	}
} 

/**
 Updates number of powerups/eatibles available from each powerup/eatible 
 */
void SimpleHero::updateReachable(GraphMap* map) {

	//For all eatibles still reachable on the map, assess how many they can reach. This should be only called on special or necessary occasions!
	for (int i = 0; i < this->numEatibles; i++ ) {
		std::pair <int,int> thisEatible;
		
		if ( this->eatibleInfo[i][5] == 1 ) {
			// Sets x/y of this eatible.
			thisEatible.first = this->eatibleInfo[i][0];
			thisEatible.second = this->eatibleInfo[i][1];

			//Asseses how many other relevant items are assesible from this eatible.
			this->assessReachable(map, thisEatible, i, 0); 
			
			//printf("Eatible Vertice: %d X: %d Y: %d, EReach: %d PReach: %d ReachAll: %d \n", this->eatibleInfo[i][2], this->eatibleInfo[i][0], this->eatibleInfo[i][1], this->eatibleInfo[i][3], this->eatibleInfo[i][4], this->eatibleInfo[i][6]);
		}
	}
	
	//For all powerups on the map, check how many other things they can reach.
	for (int i = 0; i < this->numPowerups; i++ ) {
		std::pair <int,int> thisPowerup;
		
		if ( this->powerupInfo[i][5] == 1 ) {
			// Sets x/y of this eatible.
			thisPowerup.first = this->powerupInfo[i][0];
			thisPowerup.second = this->powerupInfo[i][1];
			
			//printf("%d X: %d Y: %d\n", i, thisPowerup.first, thisPowerup.second);
		
				//Asseses how many other relevant items are assesible from this eatible.
				this->assessReachable(map, thisPowerup, i, 1); 
			
			/*
			printf("Eatible Vertice: %d X: %d Y: %d, EReach: %d PReach: %d\n", this->powerupInfo[i][2], this->powerupInfo[i][0], this->powerupInfo[i][1], this->powerupInfo[i][3], this->powerupInfo[i][4]);
			*/
		}
	}
}

/** 
Checks if there are enemies nearby to potentially avoid them??
*/
void SimpleHero::checkForEnemies(GraphMap* map, std::pair<int,int> rootNode){
// FIXME: Do something with this
}

/** 
Update all info about Eatibles/Powerups based on our location -- Essentially if we're on a target, mark it as eaten and such.
*/
void SimpleHero::updateTargets(GraphMap* map, std::pair<int,int> root) {
	int currentVertex;
	currentVertex = map->getVertex(root.first, root.second);
	
	//See if we're on an eatible. If so update info for that eatible and max eatibles
	for ( int i = 0; i < this->numEatibles; i++ ) {
		if ( currentVertex == this->eatibleInfo[i][2] && this->eatibleInfo[i][5] == 1) {
			//printf("ate eatible\n");
			this->eatibleInfo[i][5] = 0;
			this->maxEReachable--;
			this->updateReachableNoSearch(0, 1);
			this->inPursuit = 0;
		}
	}
	
	//Eaten all eatibles. Moving on!
	if ( this->maxEReachable == 0 && this->stage != 2) {
		//printf("Switched stages\n");
		this->stage = 2;
	}
	
	// Hit stage 2. Let's update things. Could remove these calls to be more efficient, but there are possible maps where powerups could go from fully accesible to at trap. We aren't expecting HUGE maps with only powerups. If we get one, We'll be computationally taxed to the bejeesus.
	if ( this->stage == 2 ) {
		//printf("updated powerups\n");
		locatePowerups(map);
		//Not sure if we ever need this.
		updateReachable(map);
	}	
		
	//See if we're on a powerup. If so mark appropriately
	for ( int i = 0; i < this->numPowerups; i++ ) {
		if ( currentVertex == this->powerupInfo[i][2] && this->powerupInfo[i][5] == 1) {
			//printf("at powerup at vertex: %d\n", currentVertex);
			this->powerupInfo[i][5] = 0;
			this->maxPReachable--;
			this->updateReachableNoSearch(1, 0);
			this->inPursuit = 0;
		}
	}
	
	//Safety check. Sometimes the map glitches and we think we're on the guy but we aren't. So instead of getting trapped following the powerup, we just make ourself wait 1 move and then continue. Works every time! ;)
	if ( maxPReachable <= 0 && maxEReachable <= 0) {
			//We're actually in a segregated map, so we'll just stop moving.
			if ( this->isSegregated ) {
				return;
			}
		this->maxPReachable = getNumPowerups(map);
		//We're willing to call these functions because we're assuming we're on a nearly empty map. This should be relatively inexpensive.
		locatePowerups(map);
		updateReachable(map);
		this->shouldWait = 1;
	}

}

/**
	Executes a BFS scan of the map from the given item location. Basically checks if this item is isolated or not.
	
	I referenced the psuedocode on http://en.wikipedia.org/wiki/Breadth-first_search to implement this method and the breadth first searches used in this program.
	
	I would have referenced our in class example but it did not sync with this computer.
*/
void SimpleHero::assessReachable(GraphMap* map, std::pair<int,int> root, int j, int isPowerup)
{
	//printf("Called assesssReachable\n");
	int currentVertex, eatiblesReachable = 0, powerupsReachable = 0;
	int* beenThere = (int*) calloc(map->getNumVertices(), sizeof(int));

	std::queue< std::pair<int,int> >* myQ = new std::queue< std::pair<int,int> >();
	
	currentVertex = map->getVertex(root.first, root.second);
	myQ->push(root);
	beenThere[currentVertex] = 1;
	
	while ( myQ->size() > 0 ) {
		std::pair<int,int> current = myQ->front();
		currentVertex = map->getVertex(current.first, current.second);
		beenThere[currentVertex] = 1;
		
		
		// See if we're on an eatible. If so ++
		for ( int i = 0; i < this->numEatibles; i++ ) {
			if ( currentVertex == this->eatibleInfo[i][2] && this->eatibleInfo[i][5] == 1) {
				//if ( isPowerup == -1 ) { printf("found eatible at X: %d Y: %d\n", this->eatibleInfo[i][0], this->eatibleInfo[i][1]); }
				eatiblesReachable++;
			}
		}
		
		//See if we're on a powerup. If so ++
		for ( int i = 0; i < this->numPowerups; i++ ) {
			if ( currentVertex == this->powerupInfo[i][2] && this->powerupInfo[i][5] == 1) {
				powerupsReachable++;
			}
		}
		
		//Add all of the neighbors of our current position
		for (int i = 0; i < map->getNumNeighbors(current.first, current.second); i++) {
			std::pair<int, int> thisNeighbor;
			
			//Look at the ith neighbor
			map->getNeighbor(current.first, current.second, i, thisNeighbor.first, thisNeighbor.second);
			
			//Set the ith neighbors vertex to current vertex
			currentVertex = map->getVertex(thisNeighbor.first, thisNeighbor.second);
			
			if ( beenThere[currentVertex] != 1 ) {
				myQ->push(thisNeighbor);
				beenThere[currentVertex] = 1;
			}
		}
		
		if (myQ->size() != 0 ) {
			myQ->pop();
		}
	}
	
	//special case: Called from our character. This is to ensure accuracy on divided maps w/ multiple heros.
	if ( isPowerup == -1 ) {
		this->maxPReachable = powerupsReachable;
		this->maxEReachable = eatiblesReachable;
	} else if ( isPowerup == 1 ) {
		this->powerupInfo[j][3] = eatiblesReachable;
		this->powerupInfo[j][4] = powerupsReachable;
		if ( powerupsReachable == this->maxPReachable && eatiblesReachable == this->maxEReachable ) {
			this->powerupInfo[j][6] = 1;
		}
	} else {
		this->eatibleInfo[j][3] = eatiblesReachable;
		this->eatibleInfo[j][4] = powerupsReachable;
		if ( powerupsReachable == this->maxPReachable && eatiblesReachable == this->maxEReachable ) {
			this->eatibleInfo[j][6] = 1;
		}
	}
	
	delete myQ;
	free(beenThere);
}

/**
	Executes a BFS to determine what to grab next.
	
	I referenced the psuedocode on http://en.wikipedia.org/wiki/Breadth-first_search to implement this method and the breadth first searches used in this program.
	
	I would have referenced our in class example but it did not sync with this computer.
*/
void SimpleHero::getNextTarget(GraphMap* map, std::pair<int,int> root)
{
	//printf("Called getNextTarget\n");
	
	int currentVertex, prevVertex, rootVertex, tempVertex = -1, itemFound = 0;
	int** beenThere = (int**) malloc(map->getNumVertices() * sizeof(int*));
	std::pair<int, int> myCoord;
	
	for ( int i = 0; i < map->getNumVertices(); i++ ) {
		beenThere[i] = (int*) calloc(2, sizeof(int));
	}

	std::queue< std::pair<int,int> >* myQ = new std::queue< std::pair<int,int> >();
	
	
	currentVertex = map->getVertex(root.first, root.second);
	/*
	printf("Started at: X: %d Y: %d\n", root.first, root.second);
	printf("Which is vertex: %d\n", currentVertex);
	*/
	rootVertex = currentVertex;
	myQ->push(root);
	beenThere[currentVertex][0] = 1;
	beenThere[currentVertex][1] = currentVertex;
	
	//printf("Going in!\n");
	
	// Go through and do our BFS to locate our item. When we locate our item we simply break.
	while ( myQ->size() > 0 ) {
		std::pair<int,int> current = myQ->front();
		currentVertex = map->getVertex(current.first, current.second);
		prevVertex = currentVertex;
		beenThere[currentVertex][0] = 1;
		
		if ( this->isSegregated ) {
			//This defaults if neither of those happened
			for ( int i = 0; i < this->numEatibles; i++ ) {
				//If we're looking at an eatible
				if ( currentVertex == this->eatibleInfo[i][2] && this->eatibleInfo[i][5] == 1 ) {
					//printf("Found eatable at: X: %d Y: %d with %d reachable\n", this->eatibleInfo[i][0], this->eatibleInfo[i][1], this->eatibleInfo[i][3]);
					itemFound = 1;
					break;
				}
			}

			for ( int i = 0; i < this->numPowerups; i++ ) {
				//If we're looking at a powerup
				if ( currentVertex == powerupInfo[i][2] && powerupInfo[i][5] == 1 ) {
					itemFound = 1;
					break;
				}
			}		
		} else if ( this->stage == 1 ) {
			for ( int i = 0; i < this->numEatibles; i++ ) {
				//If we're looking at an eatible that can access all other items.
				if ( currentVertex == this->eatibleInfo[i][2] && this->eatibleInfo[i][6] == 1 && this->eatibleInfo[i][5] == 1 ) {
					//printf("Found eatable at: X: %d Y: %d with %d reachable\n", this->eatibleInfo[i][0], this->eatibleInfo[i][1], this->eatibleInfo[i][3]);
					//printf("found eatible\n");
					itemFound = 1;
					break;
				}
			}
		} else if ( this->stage == 2 ) { 
			for ( int i = 0; i < this->numPowerups; i++ ) {
				//If we're looking at a powerup that can access all other items.
				if ( currentVertex == powerupInfo[i][2] && powerupInfo[i][6] == 1 && powerupInfo[i][5] == 1 ) {
					//printf("Found powerup in getNextTarget on stage two\n");
					//printf("Found powerup at vertex: %d and we started at vertex: %d\n", currentVertex, rootVertex);
					itemFound = 1;
					break;
				}
			}
		} 
		
		if ( itemFound ) {
			break;
		}
		
		
		//Add all of the neighbors of our current position
		for (int i = 0; i < map->getNumNeighbors(current.first, current.second); i++) {
			std::pair<int, int> thisNeighbor;
			
			//Look at the ith neighbor
			map->getNeighbor(current.first, current.second, i, thisNeighbor.first, thisNeighbor.second);
			
			//Set the ith neighbors vertex to current vertex
			currentVertex = map->getVertex(thisNeighbor.first, thisNeighbor.second);
			
			if ( beenThere[currentVertex][0] != 1 ) {
				myQ->push(thisNeighbor);
				beenThere[currentVertex][0] = 1;
				beenThere[currentVertex][1] = prevVertex;
			}
		}
		
		if (myQ->size() != 0 ) {
			myQ->pop();
		}
	}
	
	//printf("pulled out!\n");
	
	//printf("Making path\n");
	tempVertex = currentVertex;
	
	//Now develop (backwards) path from location root.
	while ( tempVertex != rootVertex ) {
		map->getPosition(tempVertex, myCoord.first, myCoord.second);
		//printf("Target set for: X: %d Y: %d\n", myCoord.first, myCoord.second);
		this->myPath->push(myCoord);
		tempVertex = beenThere[tempVertex][1];
	}
	
	//printf("path made\n");
	
	//printf("doing stuff\n");
	std::pair<int,int> tmpPair;
	
	//Sometimes some weird stuff happens and we find a powerup where we are sitting (generally happens if two powerups are sitting where we are I believe)
	if (this->myPath->size() == 0 ) {
		//printf("happened\n");
		this->myPath->push(root);
		//this->myPath->push(root);
	} else { 
		tmpPair = this->myPath->top();
	}
	//printf("done\n");
	
	for ( int i = 0; i < map->getNumVertices(); i++ ) {
		free( beenThere[i] );
	}
	
	free(beenThere);
	
	
	beenThere = 0;
	
	delete myQ;
}

#endif



