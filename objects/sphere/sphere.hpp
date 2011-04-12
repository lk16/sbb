#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <cmath>
#include <vector>

<<<<<<< HEAD
#include "../../engine/engine.hpp"
#include "../../engine/collision_test.hpp"
#include "../../engine/engine_interfaces.hpp"
#include "../../objects/finish/finish.hpp"
#include "../../engine/util.hpp"
=======
#include "engine/engine.hpp"
#include "engine/collision_test.hpp"
#include "engine/engine_interfaces.hpp"
#include "objects/finish/finish.hpp"
>>>>>>> 7d795d9ad1de932c75971aa5975d8b47383a0bc9

struct t_floor;

class player_ball:
	public can_collide<player_ball,COL_SPHERE>
{
<<<<<<< HEAD

=======
	enum doubly_owned_bool{ 
		DOB_False=0,
		DOB_True=1,
		DOB_TRY_FALSE,
		DOB_TRY_TRUE 
	};
>>>>>>> 7d795d9ad1de932c75971aa5975d8b47383a0bc9
	
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
