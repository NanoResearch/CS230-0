/*
 * GameManager.hpp
 *
 *  Created on: Mar 7, 2014
 *      Author: stolee
 */

#ifndef GAMEMANAGER_HPP_
#define GAMEMANAGER_HPP_

#include <time.h>
#include <ncurses.h>
#include <stdio.h>
#include "Actor.hpp"
#include "GraphMap.hpp"

#define GOAL_MODE_HEROWINS 1
#define GOAL_MODE_ENEMYWINS 2
#define GOAL_MODE_POWERUP_SURVIVES 3

class GameManager
{
	private:
		GraphMap* graph;

		int num_actors;
		int size_actors;
		Actor** actors;

		Actor* hero;
		Actor* enemy;
		Actor* eatable;
		Actor* powerup;

		/**
		 * These are set by command-line arguments
		 */
		int cur_move;
		int num_heroes;
		int num_eatable;
		int start_num_eatables;
		int num_enemies;
		double actor_time;
		clock_t actor_clock;

		bool enemies_enabled;

		clock_t start_clock;
		time_t start_time;
		time_t max_time; /* set by "--time" */
		bool render_all; /* true unless "--render-off" in arguments */
		int max_num_moves; /* set by "--moves #" */
		int delay_in_ms; /* set by "--delay #" */
		int delay_hero; /* set by "--delay-hero #" */
		int delay_enemy; /* set by "--delay-enemy #" */
		int delay_powerup; /* set by "--delay-powerup #" */
		int delay_eatable; /* set by "--delay-eatable #" */

		int run_points;
		int finish_points;
		int goal_mode;
		char* test_name;

		// Special message for ending
		char* message_buffer;
		void writeEndMessageAndWait();

		void drawPos( WINDOW* w, int x, int y );

		/**
		 * Render the map, using the current set of actors
		 * and positions
		 */
		void render( WINDOW* w );

	public:
		/**
		 * Construct a game manager using command-line arguments.
		 */
		GameManager( int argc, char** argv, Actor**& actors, int& num_actors );

		virtual ~GameManager();

		/**
		 * This makes a shallow copy of an actor in the list.
		 * However, it takes ownership and will delete the actor when through.
		 *
		 * This allows you to use a line such as: manager->addActor(new Actor());
		 */
		void addActor( Actor* actor );

		int getNumActors();
		Actor* getActor( int i );

		/**
		 * Call this to start the game!
		 */
		void play();
};

#endif /* GAMEMANAGER_HPP_ */
