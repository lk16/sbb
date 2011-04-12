#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include <vector>

#include "../../engine/engine.hpp"
#include "../../engine/collision_test.hpp"
#include "../../engine/engine_interfaces.hpp"
#include "../../objects/finish/finish.hpp"
#include "../../engine/util.hpp"

struct t_floor;

class player_ball:
	public can_collide<player_ball,COL_SPHERE>
{

	
	doubly_owned_bool prevcol;
	int time;
	
	
	public:
		player_ball(t_engine&,double,double,double);
		~player_ball();
		
		void collide(collision_data<t_floor>&);
		void collide(base_plane&);
		void collide(finish);
		
		void key_pressed(unsigned);
		void step();
		void generate_trs(unsigned);

		virtual void draw();
};
		
#endif
