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
	this->curX = 0;
	this->curY = 0;
	this->numEatibles = 0;
	this->numPowerups = 0;
	this->numEnemies = 0;
	this->maxEReachable = 0;
	this->maxPReachable = 0;
	this->firstRun = 1;
	this->currentTarget = -1;
	this->shouldWait = 0;
	this->isSegregated = 0;
	this->moveCount = 0;
	
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
	
	//Free enemy array
	for (int i = 0; i < this->numEnemies; i++ ){
		free(this->enemyInfo[i]);
	}
	
	this->enemyInfo = 0;
	
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
	
	Smart Hero:
		1) Just confirm checkForEnemies is working properly.
			-- Coordinated Strategies?? - Some way to assess who goes where.
			
		
	Smart Powerup: 
		- Once smart hero is done, implement it similarly. Essentially, always follow the enemy (where smarthero follows powerups) -- and always avoid the heros (where smarthero avoids enemies!)	

	Smart enemy: 
		Find nearest enemy (one call.) Then find the nearest eatible to that powerup. Go there.
		Move to closest eatible. 
		
	Smart Powerup: 
		- Once smart hero is done, implement it similarly. Essentially, always follow the enemy (where smarthero follows powerups) -- and always avoid the heros, and NEVER step on them!
	

	FIXME: implement enemy avoidance strategy (If enemey is within 2 squares, do nothing but note position. Obviouslly if we're about to collide with an enemy go the opposite direction. if enemy is within 1 square and moved toward us and could collide with us, move away from the enemy for one turn and keep moving forward??)
	
*/
int SimpleHero::selectNeighbor( GraphMap* map, int x, int y )
{	
	std::pair <int,int> myLocation;
	int myMove = 0;
	
	myLocation.first = x;
	myLocation.second = y;
	
	this->curX = x;
	this->curY = y;

	// We want to set up our hero and assess the situation of the entire map before deciding to make any moves.
	if (this->firstRun == 1) {
	
		//Count the number of eatibles and powerups
		this->numEatibles = this->getNumEatibles(map);
		this->numPowerups = this->getNumPowerups(map);
		this->numEnemies = this->getNumEnemies(map);
		
		
		//Set max reachable powerup/eatible values. This may be inaccurate at first for segregated maps.
		this->maxEReachable = this->numEatibles;
		this->maxPReachable = this->numPowerups;

		//Make our information arrays
		this->initializeEatible(this->numEatibles);
		this->initializePowerup(this->numPowerups);
		this->initializeEnemy(this->numEnemies);
		
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
	}
	
	if ( moveCount % 25 == 0 ) {
		updateReachable(map);
	}
	
	//Updates the powerups/eatibles if we are on the location of one (i.e, we just ate one)
	this->updateTargets(map, myLocation);
	this->getNextTarget(map, myLocation);
	this->chooseMove(map, myMove, myLocation);
	
	//printf("E: %d, P: %d\n", this->maxEReachable, this->maxPReachable);
	
	this->moveCount++;
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
	int numSpots = map->getNumActors();
	int eatibles = 0;
	
	for (int i = 0; i < numSpots; i++) {
		if ( (map->getActorType(i) & ACTOR_EATABLE) && !(map->getActorType(i) & ACTOR_POWERUP) ) {
			eatibles++;
		}
	}
	
	return eatibles;
}

// Counts all of the enemies on the map
int SimpleHero::getNumEnemies(GraphMap* map)
{
	int numSpots = map->getNumActors();
	int enemies = 0;
	
	for (int i = 0; i < numSpots; i++) {
		if ( map->getActorType(i) & ACTOR_ENEMY ) {
			enemies++;
		}
	}
	
	return enemies;
}

void SimpleHero::updateReachableNoSearch(int powerup, int eatible){

	 //Check through all powerups
	 for (int i = 0; i < this->numPowerups; i++ ) {
		//If the powerups still on the map
		if ( this->powerupInfo[i][5] == 1 ) {

			//If we can reach all eatibles, subtract one
			if ( this->powerupInfo[i][3] == this->maxEReachable ) {
				this->powerupInfo[i][3] -= eatible;
			}
			
			//if we can reach all powerups, subtract one.
			if ( this->powerupInfo[i][4] == this->maxPReachable ) {
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
			
			//If we can reach all eatibles, subtract one
			if ( this->eatibleInfo[i][3] == this->maxEReachable ) {
				this->eatibleInfo[i][3] -= eatible;
			}
			
			//if we can reach all powerups, subtract one.
			if ( this->eatibleInfo[i][4] == this->maxPReachable ) {
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

// Locates Vertice and X/Y coordinates of enemies
void SimpleHero::locateEnemies(GraphMap* map)
{
	int numSpots = map->getNumActors();
	int actor;
	int eCount = 0;
	int x, y;
	
	for (int i = 0; i < numSpots; i++) {
		actor = map->getActorType(i);
		if ( (actor & ACTOR_ENEMY) ) {
			map->getActorPosition( i, x, y );
			
			//Determine the direction of the enemy
			if ( x < enemyInfo[eCount][0] ) {
				this->enemyInfo[eCount][3] = -1;
				this->enemyInfo[eCount][4] = 0;
			} else if ( x > enemyInfo[eCount][0] ) {
				this->enemyInfo[eCount][3] = 1;
				this->enemyInfo[eCount][4] = 0;
			} else if ( y < enemyInfo[eCount][1] ) {
				this->enemyInfo[eCount][4] = -1;
				this->enemyInfo[eCount][3] = 0;
			} else if ( y > enemyInfo[eCount][1] ) {
				this->enemyInfo[eCount][4] = 1;
				this->enemyInfo[eCount][3] = 0;
			}
			
			
			this->enemyInfo[eCount][0] = x;
			this->enemyInfo[eCount][1] = y;
			this->enemyInfo[eCount][2] = map->getVertex(x,y);
			eCount++;
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

/** Initialize our Enemy array. Just doing this to modularize code

	Idea of the enemu array:
	
		Per each index:
	[X coordinate] [Y coordinate] [ Vertice # ] [ x direction ] [ y direction ]

*/
void SimpleHero::initializeEnemy(int numEnemies)
{
	this->enemyInfo = (int**) malloc(numEnemies * sizeof(int*));
	
	for (int i = 0; i < numEnemies; i++) {
		this->enemyInfo[i] = (int*) malloc(5 * sizeof(int));
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
	int numSpots = map->getNumActors();
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
	int goTo = map->getNumNeighbors(myLocation.first, myLocation.second);
	int* dontGo = (int*) calloc(goTo, sizeof(int));
	int avoid = 0;
	
	this->locateEnemies(map);
	
	if ( this->isSegregated && this->maxPReachable == 0 && this->maxEReachable == 0 ) {
		myMove = 0;
		return;
	}
	
	std::pair<int,int> tempCoord = this->myPath->top();
	int myX, myY, nextX, nextY;
	
	myX = tempCoord.first;
	myY = tempCoord.second;
	
	 //Check through all neighbors for enemies present/encroaching first -- This is our last chance check in case we made a mistake or something changed last minute!
	for ( int i = 0; i < goTo; i++ ) {
		map->getNeighbor(myLocation.first, myLocation.second, i, nextX, nextY);
		
		// Found an enemy at/going into this block.
		if ( checkForEnemies(nextX, nextY) ) {
			
			//What really matters -- Is there an enemy at/going into the block we're TRYING to go into?
			if ( nextX == myX && nextY == myY ) {
				avoid = 1;
			}
			
			//Just keep track of this for decision making.
			dontGo[i] = 1;
		}
		
	} 
	
	
	for ( int i = 0; i < goTo; i++ ) {
		map->getNeighbor(myLocation.first, myLocation.second, i, nextX, nextY);
		
		if ( this->shouldWait ) {
			myMove = i;
			this->shouldWait = 0;
			break;
		} 
		//Well we know where we want to go.. But we can't.
		else if ( myX == nextX && myY == nextY && !avoid) {
			myPath->pop();
			myMove = i;
			break;
		} 
		//Okay so we can't go where we want, but we can go here!
		else if (avoid && !dontGo[i]) {
			myMove = i;
			break;
		}
		
	}
	
	free(dontGo);
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
			
			//Asseses how many other relevant items are assesible from this eatible.
			this->assessReachable(map, thisPowerup, i, 1); 
		}
	}
}

//Determines if our path has enemies on it or encroaching on it.
int SimpleHero::hasEnemies(GraphMap* map, int** beenThere, int currentVertex, int root){
	int x, y;
	
	this->locateEnemies(map);
	
	while ( currentVertex != root ) {
		//printf("%d - \n", currentVertex);
		map->getPosition(currentVertex, x, y);
		

		if ( checkForEnemies(x, y) ) {
			return 1;
		}
			
		currentVertex = beenThere[currentVertex][1];
	}
	
	return 0;
}

/**
Checks if there is either
		a) an enemy on the path
		b) If we're within 2 blocks, if there is an enemy about to step into our path.
		
		FIXME: Make sure this is always working correctly.
*/
int SimpleHero::checkForEnemies(int x, int y){
	int xDist, yDist;
	
	xDist = x - curX;
	yDist = y - curY;
	
	if (xDist < 0){
		xDist *= -1;
	}
	
	if (yDist < 0){
		yDist *= -1;
	}
	
	for (int i = 0; i < this->numEnemies; i++) {
		
		if ( yDist <= 2 || xDist <= 2 ) {
		
			//Found an enemy directly on our path.
			if ( x == this->enemyInfo[i][0] && y == this->enemyInfo[i][1] ) {
				//printf("Directly on path!\n");
				return 1;
			}
		
			//If an enemy is to the left of our path, and moving right.(or just really close!)
			if ( x - 1 == this->enemyInfo[i][0] && y == this->enemyInfo[i][1] && ( this->enemyInfo[i][3] == 1 || ( xDist == 1 && yDist == 0 ) ) ) {
				//printf("LEFT\n");
				return 1;
			}

			//If an enemy is to the Right of our path, and moving Left.(or just really close!)
			if ( x + 1 == this->enemyInfo[i][0] && y == this->enemyInfo[i][1] && ( this->enemyInfo[i][3] == -1 || ( xDist == 1 && yDist == 0 ) ) ) {
				//printf("RIGHT!\n");
				return 1;
			}

			//If an enemy is below our path, and moving up. (or just really close!)
			if ( x == this->enemyInfo[i][0] && y + 1 == this->enemyInfo[i][1] && ( this->enemyInfo[i][4] == - 1 || ( xDist == 0 && yDist == 1 ) ) ) {
				//printf("BOTTOM!\n");
				return 1;
			}

			//If an enemy is above our path, and moving down. (or just really close!)
			if ( x == this->enemyInfo[i][0] && y - 1 == this->enemyInfo[i][1] && ( this->enemyInfo[i][4] == 1 || ( xDist == 0 && yDist == 1 ) ) ) {
				//printf("TOP!\n")
				return 1;
			}
		}
	}
	
	
	return 0;
	
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
		}
	}
		
	locatePowerups(map);	
		
	// Hit stage 2. Let's update things. Could remove these calls to be more efficient, but there are possible maps where powerups could go from fully accesible to at trap. We aren't expecting HUGE maps with only powerups. If we get one, We'll be computationally taxed to the bejeesus.
	//if ( this->stage == 2 ) {
		//printf("updated powerups\n");
		
		//Not sure if we ever need this.
		//updateReachable(map);
	//}	
		
	//See if we're on a powerup. If so mark appropriately
	for ( int i = 0; i < this->numPowerups; i++ ) {
		if ( currentVertex == this->powerupInfo[i][2] && this->powerupInfo[i][5] == 1) {
			//printf("at powerup at vertex: %d\n", currentVertex);
			this->powerupInfo[i][5] = 0;
			this->maxPReachable--;
			this->updateReachableNoSearch(1, 0);
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
		
		
			for ( int i = 0; i < this->numEatibles; i++ ) {
				//If we're looking at an eatible
				if ( currentVertex == this->eatibleInfo[i][2] && this->eatibleInfo[i][5] == 1 && (this->eatibleInfo[i][6] == 1 || this->isSegregated == 1) ) {
					if (!this->hasEnemies(map, beenThere, currentVertex, rootVertex)) {
						itemFound = 1;
						break;
					}
				}
			}

			for ( int i = 0; i < this->numPowerups; i++ ) {
				//If we're looking at a powerup
				if ( currentVertex == powerupInfo[i][2] && powerupInfo[i][5] == 1 ) {
					if (!this->hasEnemies(map, beenThere, currentVertex, rootVertex)) {
					
						//Reset it's value and check again.
						this->powerupInfo[i][6] = 0;
						std::pair <int,int> thisPowerup;
						thisPowerup.first = this->powerupInfo[i][0];
						thisPowerup.second = this->powerupInfo[i][1];
						this->assessReachable(map, thisPowerup, i, 1); 
						
						//Now we make sure it's ACTUALLY not isolated (because the first check can give a false positive!)
						if ( this->powerupInfo[i][6] == 1  || this->isSegregated == 1) {
							itemFound = 1;
							break;
						}
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
	
	tempVertex = currentVertex;
	
	//Now develop (backwards) path from location root.
	while ( tempVertex != rootVertex ) {
		map->getPosition(tempVertex, myCoord.first, myCoord.second);
		//printf("Target set for: X: %d Y: %d\n", myCoord.first, myCoord.second);
		this->myPath->push(myCoord);
		tempVertex = beenThere[tempVertex][1];
	}
	

	std::pair<int,int> tmpPair;
	
	//Sometimes some weird stuff happens and we find a powerup where we are sitting (generally happens if two powerups are sitting where we are I believe)
	if (this->myPath->size() == 0 ) {
		this->myPath->push(root);
	} else { 
		tmpPair = this->myPath->top();
	}
	
	for ( int i = 0; i < map->getNumVertices(); i++ ) {
		free( beenThere[i] );
	}
	
	free(beenThere);
	
	
	beenThere = 0;
	
	delete myQ;
}

#endif



