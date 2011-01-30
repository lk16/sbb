#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include <vector>

#include "engine/engine.hpp"
#include "engine/collision_test.hpp"
#include "engine/engine_interfaces.hpp"
#include "objects/finish/finish.hpp"

struct t_floor;

class player_ball:
	public can_collide<player_ball,COL_SPHERE>
{
	enum doubly_owned_bool{ 
		DOB_False=0,
		DOB_True=1,
		DOB_TRY_FALSE,
		DOB_TRY_TRUE 
	};
	
	doubly_owned_bool prevcol;
	int time;
	
	
	public:
		player_ball(t_engine&,double,double,double);
		
		void collide(collision_data<t_floor>&);
		void collide(base_plane&);
		void collide(finish);
		
		void key_pressed(unsigned);
		void step();
		void generate_trs(unsigned);

		virtual void draw();
};
		
#endif
