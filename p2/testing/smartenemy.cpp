/*
 * SmartEnemy.cpp
 * 	Author: Paul Gerlich
 */
 
 #ifndef _SmartEnemyCPP_
 #define _SmartEnemyCPP_

#define ISEATABLE 4
#define ISPOWERUP 8
 
 
#include "smartenemy.hpp"
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <queue>

SmartEnemy::SmartEnemy( int type ) : Actor(type)
{
	this->curX = 0;
	this->curY = 0;
	this->numEatibles = 0;
	this->numHeros = 0;
	this->numEnemies = 0;
	this->firstRun = 1;
	this->moveCount = 0;
	this->direction = 0;
	
	this->myPath = new std::stack< std::pair<int, int> >();
}

//Simple destructor
SmartEnemy::~SmartEnemy()
{
	//printf("started\n");
	// Free eatible array
	for (int i = 0; i < this->numEatibles; i++) {
		free(this->eatibleInfo[i]);
	}
	
	//free(this->eatibleInfo);
	
	this->eatibleInfo = 0;
	
		//Free enemy array
	for (int i = 0; i < this->numHeros; i++ ){
		free(this->heroInfo[i]);
	}
	
	this->heroInfo = 0;
	
	delete this->myPath;
	
	this->myPath = 0;

}

/**
This will run based on this basic idealogy:

	We basically decide if we are the only enemy. If so, we just pursue the hero.
	If there are two or more enemies, One grabs the nearest eatible, and the others chase him based on dir
	
	Based on these basics, we decide which block to go to next.

	Smart enemy: 
		Options: Have % numEnemy options or something? 
							
							
			Strategy:
				Direction = 0, want to take shortest path to hero
				Direction = 1, want to take second shortest path
				Direction = 2, want to take third shortest path
				direction = 3, want to take fourth shortest path
	
*/
int SmartEnemy::selectNeighbor( GraphMap* map, int x, int y )
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
		this->numHeros = this->getNumHeros(map);

		//Make our information arrays
		this->initializeEatible(this->numEatibles);
		this->initializeHero(this->numHeros);
		
		//Locate the eatibles
		this->locateEatibles(map);
		
		//Don't do this firstrun stuff again
		this->firstRun = 0;
		
	}
	
	this->locateHeros(map);

	
	//Sets next vertex (closest to master) -- if we are the master, it just stars moving
	this->getNextTarget(map, myLocation);
	
	
	this->chooseMove(map, myMove, myLocation);
	
	this->moveCount++;
	return myMove;
}

// Return a new copy of our hero
Actor* SmartEnemy::duplicate()
{
	return new SmartEnemy(this->getType());
}

// Return Net ID
const char* SmartEnemy::getNetId()
{
	return "pgerlich";
}

//Return Actor ID
const char* SmartEnemy::getActorId()
{
	return "smartenemy";
}

// Counts all of the eatibles on the map
int SmartEnemy::getNumEatibles(GraphMap* map)
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

// Counts all of the enemies/heros on the map
int SmartEnemy::getNumHeros(GraphMap* map)
{
	int numSpots = map->getNumVertices();
	int heros = 0;
	int x, y;
	
	for (int i = 0; i < numSpots; i++) {
		if ( map->getActorType(i) & ACTOR_HERO ) {
			heros++;
		}
		
		if ( map->getActorType(i) & ACTOR_ENEMY ) {
			map->getActorPosition(i, x, y);
			
			if ( x == curX && y == curY ) {
				this->direction = i % 4;
			}
			
			this->numEnemies++;
		}
	}
	
	return heros;
}

// Locates Vertice and X/Y coordinates of eatibles
void SmartEnemy::locateEatibles(GraphMap* map)
{
	int numSpots = map->getNumVertices();
	int actor;
	int eatCount = 0;
	int count = 0;
	int x, y;
	
	for (int i = 0; i < numSpots; i++) {
		actor = map->getActorType(i);
		if ( (actor & ACTOR_EATABLE) && !(actor & ACTOR_POWERUP) ) {
			map->getActorPosition( i, x, y );
			this->eatibleInfo[eatCount][0] = x;
			this->eatibleInfo[eatCount][1] = y;
			this->eatibleInfo[eatCount][2] = map->getVertex(x,y);
			
			if ( x == -1 && y == -1 ) {
				this->eatibleInfo[eatCount][5] = 0;
			} else {
				this->eatibleInfo[eatCount][5] = 1;
				count++;
			}
			
			eatCount++;
		}
	}

}

// Locates Vertice and X/Y coordinates of enemies
void SmartEnemy::locateHeros(GraphMap* map)
{
	int numSpots = map->getNumActors();
	int actor;
	int eCount = 0;
	int x, y;
	
	for (int i = 0; i < numSpots; i++) {
		actor = map->getActorType(i);
		if ( (actor & ACTOR_HERO) ) {
			map->getActorPosition( i, x, y );
			
			this->heroInfo[eCount][0] = x;
			this->heroInfo[eCount][1] = y;
			this->heroInfo[eCount][2] = map->getVertex(x,y);
			eCount++;
		}
	}
	
}

/** Initialize our Eatible array. Just doing this to modularize code

	Idea of the eatible array:
	
		Per each index:
	[X coordinate] [Y coordinate] [ Vertice # ] [ # of eatibles reachable from this powerup] [ # of powerups reachable from this powerup] [ still available] [ can reach all other target items ]

*/
void SmartEnemy::initializeEatible(int numEatibles)
{
	this->eatibleInfo = (int**) malloc(numEatibles * sizeof(int*));
	
	for (int i = 0; i < numEatibles; i++) {
		this->eatibleInfo[i] = (int*) malloc(7 * sizeof(int));
	}
	
}

/** Initialize our Enemy array. Just doing this to modularize code

	Idea of the enemu array:
	
		Per each index:
	[X coordinate] [Y coordinate] [ Vertice # ]

*/
void SmartEnemy::initializeHero(int numEnemies)
{
	this->heroInfo = (int**) malloc(numEnemies * sizeof(int*));
	
	for (int i = 0; i < numEnemies; i++) {
		this->heroInfo[i] = (int*) malloc(3 * sizeof(int));
	}
	
}


/**
Simple chooses our next move from the stack.
*/
void SmartEnemy::chooseMove(GraphMap* map, int &myMove, std::pair<int,int> myLocation) {
	int goTo = map->getNumNeighbors(myLocation.first, myLocation.second);
	
	std::pair<int,int> tempCoord = this->myPath->top();
	int myX, myY, nextX, nextY;

	myX = tempCoord.first;
	myY = tempCoord.second;
	

	
	for ( int i = 0; i < goTo; i++ ) {
		map->getNeighbor(myLocation.first, myLocation.second, i, nextX, nextY);
	
		if ( myX == nextX && myY == nextY) {
			if ( !this->myPath->empty() ) {
				this->myPath->pop();
			}
			myMove = i;
			break;
		} 
	}

}


/**
	Executes a BFS to determine what to grab next.
	
	I referenced the psuedocode on http://en.wikipedia.org/wiki/Breadth-first_search to implement this method and the breadth first searches used in this program.
	
	I would have referenced our in class example but it did not sync with this computer.
*/
void SmartEnemy::getNextTarget(GraphMap* map, std::pair<int,int> root)
{
	int currentVertex, prevVertex, rootVertex, tempVertex = -1, itemFound = 0;
	int** beenThere = (int**) malloc(map->getNumVertices() * sizeof(int*));
	std::pair<int, int> myCoord;
	int attempt = 0;
	
	for ( int i = 0; i < map->getNumVertices(); i++ ) {
		beenThere[i] = (int*) calloc(2, sizeof(int));
	}

	std::queue< std::pair<int,int> >* myQ = new std::queue< std::pair<int,int> >();
	
	
	currentVertex = map->getVertex(root.first, root.second);

	rootVertex = currentVertex;
	myQ->push(root);
	beenThere[currentVertex][0] = 1;
	beenThere[currentVertex][1] = currentVertex;

	// Go through and do our BFS to locate our item. When we locate our item we simply break.
	while ( myQ->size() > 0 ) {
		std::pair<int,int> current = myQ->front();
		currentVertex = map->getVertex(current.first, current.second);
		prevVertex = currentVertex;
		beenThere[currentVertex][0] = 1;
		
		
		if ( this->direction == 1 ) {
			for ( int i = 0; i < this->numEatibles; i++ ) {
				if ( currentVertex == eatibleInfo[i][2] ) {
					itemFound = 1;
					break;
				}
			}
		} else {
			for ( int i = 0; i < this->numHeros; i++ ) {
				//If we're looking at an eatible
				if ( currentVertex == heroInfo[i][2] ) {
					if ( attempt == this->direction || attempt == ( this->direction + 1 ) ) {
						itemFound = 1;
						break;
					} else {
						attempt++;
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



