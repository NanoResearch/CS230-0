/*
 * SimpleHero.hpp
 *
 *  Author: Paul Gerlich
 */

#ifndef _SimpleHero_
#define _SimpleHero_

#include "Actor.hpp"
#include "GraphMap.hpp"
#include <utility>
#include <stack>

/**
 * This actor will do stuff.
 */
class SimpleHero : public Actor
{
	protected:
		int numEatibles;
		int numPowerups;
		int maxEReachable;
		int maxPReachable;
		int firstRun;
		int inPursuit;
		int currentTarget;
		int stage;
		int shouldWait;
		int isSegregated;
		int** eatibleInfo;
		int** powerupInfo;
		std::stack< std::pair<int, int> >* myPath;

	public:
		SimpleHero(int type);
		virtual ~SimpleHero();
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
		
		virtual void initializeEatible(int numEatibles);
		
		virtual void initializePowerup(int numPowerups);
		
		//virtual void resetPowerupInfo(int numPowerups);
		
		virtual void locateEatibles(GraphMap* map);
		
		virtual void chooseMove(GraphMap* map, int &myMove, std::pair<int,int> myLocation);
		
		virtual void locatePowerups(GraphMap* map);
		
		virtual void assessReachable(GraphMap* map, std::pair<int,int> rootNode, int i, int isPowerup);
		
		virtual void updateReachableNoSearch(int powerUp, int eatible);
		
		virtual void getNextTarget(GraphMap* map, std::pair<int,int> rootNode);
		
		virtual void checkForEnemies(GraphMap* map, std::pair<int,int> rootNode);
		
		virtual void updateTargets(GraphMap* map, std::pair<int,int> rootNode);
		
		virtual void updateReachable(GraphMap* map);
};

#endif
