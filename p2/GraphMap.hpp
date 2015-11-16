/*
 * GraphMap.hpp
 *
 *  Created on: Mar 7, 2014
 *      Author: stolee
 */

#ifndef GRAPHMAP_HPP_
#define GRAPHMAP_HPP_

#include <stdio.h>

#define SPECIAL_HERO_SPAWN 1
#define SPECIAL_ENEMY_SPAWN 2
#define SPECIAL_EATABLE_SPAWN 4
#define SPECIAL_POWERUP_SPAWN 8

class GameManager;

/**
 * The Graph class contains a simple data structure for reading from a (directed) graph.
 *
 * It is limited to only storing out-edges and out-degree information.
 *
 * This graph is also used to store the 2D map.
 */
class GraphMap
{
		/**
		 * Give GameManager special privileges to access protected members of GraphMap.
		 */
		friend class GameManager;
	protected:
		int num_vertices;
		int* vertex_x;
		int* vertex_y;
		int** xy_vertex;
		int* out_degrees;
		int** out_edges;
		bool toroidal;

		void initGraph( int w, int h );
		void addEdge( int x, int y, int a, int b );

		int w;
		int h;

		int size_special;
		int num_special;
		int* special_vertices;
		int* special_types;

		int size_actors;
		int num_actors;
		int* actor_positions;
		int* actor_types;

		/**
		 * Modify the actors, used by GameManager
		 */
		int addActor( int type, int x, int y );
		bool moveActor( int actor, int x, int y, bool force );
		int setActorType( int i, int type );

		/**
		 * Special positions, used by GameManager.
		 */
		int getNumSpecial();
		int getSpecialType( int i );

		char** map_chars;
		char getMapChar(int x, int y);

	public:
		/**
		 * Load a graph from a file, using the game-board format.
		 */
		GraphMap( FILE* f );

		/**
		 * Destructor
		 */
		virtual ~GraphMap();

		/**
		 * Returns the number of vertices in the graph.
		 */
		int getNumVertices();

		/**
		 * Returns the number of out-neighbors for the position (x,y)
		 */
		int getNumNeighbors( int x, int y );

		/**
		 * Assigns the value of the ith neighbor of (x,y) to (a,b).
		 */
		void getNeighbor( int x, int y, int i, int& a, int& b );

		/** Returns the total number of actors in the map */
		int getNumActors();
		
		/** Gets the actor type */
		int getActorType( int i );
		
		/* Gets the x/y coordinates of the actor in the array */
		void getActorPosition( int i, int& x, int& y );

		/** Width of map */
		inline int getWidth() { return this->w; }
		
		/** Height of map */
		inline int getHeight() { return this->h; }
		
		/** Return the vertex number of that x/y position */
		int getVertex(int x, int y);
		
		/** Changes the x/y to the x/y of the given vertex */
		void getPosition(int v, int& x, int& y);

		void print();
};

#endif /* GRAPH_HPP_ */
