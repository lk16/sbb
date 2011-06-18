#ifndef SBB_PLAYER_BALL_HPP
#define SBB_PLAYER_BALL_HPP

#include <cmath>
#include <vector>
#include <algorithm>
#include <gdk/gdkkeysyms.h>

#include "engine/engine.hpp"
#include "engine/collision_test.hpp"
#include "engine/engine_interface.hpp"
#include "engine/collision_data.hpp"
#include "engine/util.hpp"
#include "objects/finish/finish.hpp"
#include "objects/floor/floor.hpp"
#include "objects/wall/wall.hpp"
#include "register/register.hpp"

class player_ball:
	public can_collide<player_ball,COL_SPHERE>
{
	
	doubly_owned_bool prevcol;
	int time;
	
	public:
		player_ball(const flomath::point&);
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
