/*
 * SmartEnemy.hpp
 *
 *  Author: Paul Gerlich
 */

#ifndef _SmartEnemy_
#define _SmartEnemy_

#include "Actor.hpp"
#include "GraphMap.hpp"
#include <utility>
#include <stack>

/**
 * This actor will do stuff.
 */
class SmartEnemy : public Actor
{
	protected:
		int curX;
		int curY;
		int numEatibles;
		int numEnemies;
		int numHeros;
		int firstRun;
		int moveCount;
		int direction;
		int** eatibleInfo;
		int** heroInfo;
		std::stack< std::pair<int, int> >* myPath;

	public:
		SmartEnemy(int type);
		virtual ~SmartEnemy();
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
		
		/* Counts total eatibles on the map */
		virtual int getNumEatibles(GraphMap* map);
		
		/* Counts total powerups on the map */
		virtual int getNumHeros(GraphMap* map);
		
		/* intialize our array of eatibles */
		virtual void initializeEatible(int numEatibles);

		/* intialize our array of enemies */
		virtual void initializeHero(int numHeros);
		
		/* Locate all the eatibles on the map */
		virtual void locateEatibles(GraphMap* map);
		
		/* Locate all the heros on the map */
		virtual void locateHeros(GraphMap* map);
		
		/* Choose our next move */
		virtual void chooseMove(GraphMap* map, int &myMove, std::pair<int,int> myLocation);
		
		/* Get next target to grab */
		virtual void getNextTarget(GraphMap* map, std::pair<int,int> rootNode);
		
};

#endif
