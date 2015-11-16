/*
 * SmartPowerup.hpp
 *
 *  Author: Paul Gerlich
 */

#ifndef _SmartPowerup_
#define _SmartPowerup_

#include "Actor.hpp"
#include "GraphMap.hpp"
#include <utility>
#include <stack>

/**
 * This actor will do stuff.
 */
class SmartPowerup : public Actor
{
	protected:
		int curX;
		int curY;
		int numHeros;
		int numEnemies;
		int firstRun;
		int** enemyInfo;
		int** heroInfo;
		std::stack< std::pair<int, int> >* myPath;

	public:
		SmartPowerup(int type);
		virtual ~SmartPowerup();
		virtual int selectNeighbor(GraphMap* map, int x, int y);
		virtual Actor* duplicate();
		 /**
		 * Report your netid through your code.
		 *
		 * Useful for later, secret purposes.
		 */
		virtual const char* getNetId();

		/**
		 * Report the name of the actor
		 */
		virtual const char* getActorId();
	
		/* Counts total powerups on the map */
		virtual int getNumEnemies(GraphMap* map);
		
		/* Counts total powerups on the map */
		virtual int getNumHeros(GraphMap* map);
		
		/* Checks if enemies are on or encroaching on our path */
		virtual int checkForHeros(int x, int y);
		
		/* Determines if any of the vertex have enemies */
		virtual int hasHeros(GraphMap* map, int** beenThere, int currentVertex, int root);
		
		/* intialize our array of powerups */
		virtual void initializeEnemy(int numPowerups);
		
		/* Returns the distance between the two vertices */
		virtual int getDistance(GraphMap* map, std::pair<int,int> vertexOne, std::pair<int,int> vertexTwo );
		
		/* intialize our array of enemies */
		virtual void initializeHero(int numPowerups);
		
		/* Locate heros on the map */
		virtual void locateHeros(GraphMap* map);
		
		/* Locate enemies on the map */
		virtual void locateEnemies(GraphMap* map);
		
		/* Choose our next move based on path/hero locations */
		virtual void chooseMove(GraphMap* map, int &myMove, std::pair<int,int> myLocation);
		
		/* Find nearest enemy to follow */
		virtual void getNextTarget(GraphMap* map, std::pair<int,int> rootNode);
};

#endif
