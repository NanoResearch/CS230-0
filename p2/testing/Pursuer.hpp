/*
 * Pursuer.hpp
 *
 *  Created on: Mar 25, 2014
 *      Author: dstolee
 */

#ifndef PURSUER_HPP_
#define PURSUER_HPP_


#include "Actor.hpp"

class Pursuer : public Actor
{
	public:
		Pursuer(int type);
		virtual ~Pursuer();
		virtual int selectNeighbor(GraphMap* map, int x, int y);
		virtual Actor* duplicate();
		virtual const char* getActorId();
};




#endif /* PURSUER_HPP_ */
