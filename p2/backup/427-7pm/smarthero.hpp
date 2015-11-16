/*
 * SmartHero.hpp
 *
 *  Author: Paul Gerlich
 */

#ifndef _SmartHero_
#define _SmartHero_

#include "Actor.hpp"
#include "GraphMap.hpp"
#include <utility>
#include <stack>

/**
 * This actor will do stuff.
 */
class SmartHero : public Actor
{
	protected:
		int curX;
		int curY;
		int numEatibles;
		int numPowerups;
		int numEnemies;
		int maxEReachable;
		int maxPReachable;
		int firstRun;
		int moveCount;
		int undecided;
		int undecidedDir;
		int currentTarget;
		int shouldWait;
		int isSegregated;
		int** eatibleInfo;
		int** powerupInfo;
		int** enemyInfo;
		std::stack< std::pair<int, int> >* myPath;

	public:
		SmartHero(int type);
		virtual ~SmartHero();
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
		virtual int getNumPowerups(GraphMap* map);
		
		/* Counts total powerups on the map */
		virtual int areUndecided();
		
		/* Counts total powerups on the map */
		virtual int getNumEnemies(GraphMap* map);
		
		/* Determines if any of the vertex have enemies */
		virtual int hasEnemies(GraphMap* map, int** beenThere, int currentVertex, int root);
		
		/* Checks if enemies are on or encroaching on our path */
		virtual int checkForEnemies(int x, int y);
		
		/* intialize our array of eatibles */
		virtual void initializeEatible(int numEatibles);
		
		/* intialize our array of powerups */
		virtual void initializePowerup(int numPowerups);
		
		/* intialize our array of enemies */
		virtual void initializeEnemy(int numPowerups);
		
		//virtual void resetPowerupInfo(int numPowerups);
		
		virtual void locateEatibles(GraphMap* map);
		
		virtual void locateEnemies(GraphMap* map);
		
		virtual void chooseMove(GraphMap* map, int &myMove, std::pair<int,int> myLocation);
		
		virtual void locatePowerups(GraphMap* map);
		
		virtual void assessReachable(GraphMap* map, std::pair<int,int> rootNode, int i, int isPowerup);
		
		virtual void updateReachableNoSearch(int powerUp, int eatible);
		
		virtual void getNextTarget(GraphMap* map, std::pair<int,int> rootNode);
		
		virtual void updateTargets(GraphMap* map, std::pair<int,int> rootNode);
		
		virtual void updateReachable(GraphMap* map);
};

#endif
