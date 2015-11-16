/*
 * SmartPowerup.cpp
 * 	Author: Paul Gerlich
 */
 
 #ifndef _SmartPowerupCPP_
 #define _SmartPowerupCPP_ 
 
#include "smartpowerup.hpp"
#include <time.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <queue>

SmartPowerup::SmartPowerup( int type ) : Actor(type)
{
	this->curX = 0;
	this->curY = 0;
	this->numEnemies = 0;
	this->numHeros = 0;
	this->firstRun = 1;
	
	this->myPath = new std::stack< std::pair<int, int> >();
}

//Simple destructor
SmartPowerup::~SmartPowerup()
{

	//Free power up array
	for (int i = 0; i < this->numHeros; i++){
		free(this->heroInfo[i]);
	}
	
	//free(this->powerupInfo);
	
	this->heroInfo = 0;
	
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

	We will act similarly to the SmartHero --> In that we will follow enemies and avoid heros. It's going to be like a SmartHero on a map with enemies and powerups, essentially. Following the powerups while avoiding the enemies.
	
	Based on these basics, we decide which block to go to next.


*/
int SmartPowerup::selectNeighbor( GraphMap* map, int x, int y )
{	
	std::pair <int,int> myLocation;
	int myMove = 0;
	
	myLocation.first = x;
	myLocation.second = y;
	
	this->curX = x;
	this->curY = y;

	// We want to set up our hero and assess the situation of the entire map before deciding to make any moves.
	if (this->firstRun == 1) {
	
		//Count the number of Heros/Enemies
		this->numEnemies = this->getNumEnemies(map);
		this->numHeros = this->getNumHeros(map);

		//Make our information arrays
		this->initializeEnemy(this->numEnemies);
		this->initializeHero(this->numHeros);
		
		//Don't do this firstrun stuff again
		this->firstRun = 0;
	}

	//Locate the relevant actors
	this->locateEnemies(map);
	this->locateHeros(map);	
	
	//Find nearest enemy and follow him!
	this->getNextTarget(map, myLocation);
	this->chooseMove(map, myMove, myLocation);

	return myMove;
}

// Return a new copy of our hero
Actor* SmartPowerup::duplicate()
{
	return new SmartPowerup(this->getType());
}

// Return Net ID
const char* SmartPowerup::getNetId()
{
	return "pgerlich";
}

//Return Actor ID
const char* SmartPowerup::getActorId()
{
	return "smartpowerup";
}

// Counts all of the enemies on the map
int SmartPowerup::getNumEnemies(GraphMap* map)
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

/** 
Counts all of the Heros on the map
*/
int SmartPowerup::getNumHeros(GraphMap* map)
{
	int numSpots = map->getNumActors();
	int numHeros = 0;
	
	for (int i = 0; i < numSpots; i++) {
		if (map->getActorType(i) & ACTOR_HERO) {
			numHeros++;
		}
	}
	
	return numHeros;
}

// Locates Vertice and X/Y coordinates of enemies
void SmartPowerup::locateEnemies(GraphMap* map)
{
	int numSpots = map->getNumActors();
	int actor;
	int eCount = 0;
	int x, y;
	
	for (int i = 0; i < numSpots; i++) {
		actor = map->getActorType(i);
		if ( (actor & ACTOR_ENEMY) ) {
			map->getActorPosition( i, x, y );
						
			this->enemyInfo[eCount][0] = x;
			this->enemyInfo[eCount][1] = y;
			this->enemyInfo[eCount][2] = map->getVertex(x,y);
			
			if ( x != -1 && y != -1 ) {
				this->enemyInfo[eCount][3] = 1;
			}
			
			eCount++;
		}
	}
	
}

// Locates Vertice and X/Y coordinates of Heros
void SmartPowerup::locateHeros(GraphMap* map)
{
	int numSpots = map->getNumActors();
	int actor;
	int hCount = 0;
	int temp = 0;
	int x, y;
	
	for (int i = 0; i < numSpots; i++) {
		actor = map->getActorType(i);
		if ( (actor & ACTOR_HERO) ) {
			map->getActorPosition( i, x, y );
			
			if ( x != -1 && y != -1 ) {
				temp++;
			}
			
			this->heroInfo[hCount][0] = x;
			this->heroInfo[hCount][1] = y;
			this->heroInfo[hCount][2] = map->getVertex(x,y);
			hCount++;
		}
	}
	
	//printf("Powerups Left: %d\n", temp);
	
}

/** Initialize our Enemy array. Just doing this to modularize code

	Idea of the enemu array:
	
		Per each index:
	[X coordinate] [Y coordinate] [ Vertice # ] [ still available ]

*/
void SmartPowerup::initializeEnemy(int numEnemies)
{
	this->enemyInfo = (int**) malloc(numEnemies * sizeof(int*));
	
	for (int i = 0; i < numEnemies; i++) {
		this->enemyInfo[i] = (int*) malloc(4 * sizeof(int));
	}
	
}


/** Initialize our hero array. Just to modularize code.

	Idea of the powerup array:
		Per each index:
	[X coordinate] [Y coordinate] [ Vertice # ]


*/
void SmartPowerup::initializeHero(int numHeros)
{
	this->heroInfo = (int**) malloc(numHeros * sizeof(int*));
	
	for (int i = 0; i < numHeros; i++) {
		this->heroInfo[i] = (int*) malloc(3 * sizeof(int));
	}
} 



/**
Simple chooses our next move from the stack.
*/
void SmartPowerup::chooseMove(GraphMap* map, int &myMove, std::pair<int,int> myLocation) {
	int goTo = map->getNumNeighbors(myLocation.first, myLocation.second);
	int* dontGo = (int*) calloc(goTo, sizeof(int));
	int avoid = 0;
	
	std::pair<int,int> tempCoord = this->myPath->top();
	int myX, myY, nextX, nextY;
	
	myX = tempCoord.first;
	myY = tempCoord.second;
	
	 //Check through all neighbors for enemies present/encroaching first -- This is our last chance check in case we made a mistake or something changed last minute!
	for ( int i = 0; i < goTo; i++ ) {
		map->getNeighbor(myLocation.first, myLocation.second, i, nextX, nextY);
		
		// Found an enemy at/going into this block.
		if ( checkForHeros(nextX, nextY) ) {
			
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
		
		//Well we know where we want to go.. But we can't.
		if ( myX == nextX && myY == nextY && !avoid) {
			if ( !this->myPath->empty() ) {
				this->myPath->pop();
			}
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


//Determines if our path has enemies on it or encroaching on it.
int SmartPowerup::hasHeros(GraphMap* map, int** beenThere, int currentVertex, int root){
	int x, y;
	
	while ( currentVertex != root ) {
		//printf("%d - \n", currentVertex);
		map->getPosition(currentVertex, x, y);
		

		if ( checkForHeros(x, y) ) {
			return 1;
		}
			
		currentVertex = beenThere[currentVertex][1];
	}
	
	return 0;
}

/**
Checks if there is either
		a) an enemy on the path
		b) If we're within 2 blocks: if there is an enemy about to step into our path.
*/
int SmartPowerup::checkForHeros(int x, int y){
	int xDist, yDist;
	int dist = 2;
	
	xDist = x - curX;
	yDist = y - curY;
	
	if (xDist < 0){
		xDist *= -1;
	}
	
	if (yDist < 0){
		yDist *= -1;
	}
	
	for (int i = 0; i < this->numHeros; i++) {
		
		if ( yDist <= dist || xDist <= dist ) {
		
			//Found an enemy directly on our path.
			if ( x == this->heroInfo[i][0] && y == this->heroInfo[i][1] ) {
				return 1;
			}
		
			//If an enemy is to the left of our path, and moving right.(or just really close!)
			if ( x - 1 == this->heroInfo[i][0] && y == this->heroInfo[i][1] ) {
				return 1;
			}

			//If an enemy is to the Right of our path, and moving Left.(or just really close!)
			if ( x + 1 == this->heroInfo[i][0] && y == this->heroInfo[i][1] ) {
				return 1;
			}

			//If an enemy is below our path, and moving up. (or just really close!)
			if ( x == this->heroInfo[i][0] && y + 1 == this->heroInfo[i][1] ) {
				return 1;
			}

			//If an enemy is above our path, and moving down. (or just really close!)
			if ( x == this->heroInfo[i][0] && y - 1 == this->heroInfo[i][1] ) {
				return 1;
			}
		}
	}
	
	
	return 0;
	
}


int SmartPowerup::getDistance(GraphMap* map, std::pair<int,int> vertexOne, std::pair<int,int> vertexTwo) {

	int x1, x2, y1, y2, yDist, xDist, xLim, yLim;
	
	x1 = vertexOne.first;
	y1 = vertexOne.second;
	x2 = vertexTwo.first;
	y2 = vertexTwo.second;
	
	xLim = ( map->getWidth() / 10 ) + 1;
	yLim = ( map->getHeight() / 10 ) + 1;
	
	yDist = y2 - y1;
	xDist = x2 - x1;
	
	if ( yDist < 0 ) {
		yDist *= -1;
	}
	
	if ( xDist < 0 ) {
		xDist *= -1;
	}
	
	if ( yDist >= yLim || xDist >= xLim ) {
		return 1;
	}
	
	return 0;
}

/**
	Executes a BFS to determine what to grab next.
	
	I referenced the psuedocode on http://en.wikipedia.org/wiki/Breadth-first_search to implement this method and the breadth first searches used in this program.
	
	I would have referenced our in class example but it did not sync with this computer.
*/
void SmartPowerup::getNextTarget(GraphMap* map, std::pair<int,int> root)
{
	int currentVertex, prevVertex, rootVertex, tempVertex = -1, itemFound = 0;
	int** beenThere = (int**) malloc(map->getNumVertices() * sizeof(int*));
	int checksOut = 1;
	std::pair<int, int> myCoord;
	
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
		
			for ( int i = 0; i < this->numEnemies; i++ ) {
				//If we're looking at a powerup
				if ( currentVertex == enemyInfo[i][2] && enemyInfo[i][3] == 1 ) {
					if ( !this->hasHeros(map, beenThere, currentVertex, rootVertex) ) {
						//Look at each hero and make sure this spot is far enough away from them.
						for ( int i = 0; i < this->numHeros; i++) {
							std::pair<int,int> heroLocation;
							map->getPosition(heroInfo[i][2], heroLocation.first, heroLocation.second);
							//If this spot is not far enough away from any one of the enemies, it doesn't check out.
							if ( !this->getDistance(map, current, heroLocation ) ) { 
								checksOut = 0;
							}
						}
						
						//If our spot checks out ( far enough away from all heros ) then we're good!
						if ( checksOut ) {
							itemFound = 1;
							break;
						} else {
							checksOut = 1;
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
		this->myPath->push(myCoord);
		tempVertex = beenThere[tempVertex][1];
	}
	
	std::pair<int,int> tmpPair;
	
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



