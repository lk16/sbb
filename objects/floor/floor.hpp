#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "engine/collision_test.hpp"
#include "engine/engine.hpp"
//alle floors moeten hier veilig van kunnen inheriten.
class t_floor:
	public can_collide<t_floor,COL_OTHER>
{
	public:
		t_floor(t_engine& e,const flomath::point& pos,const std::string& name):
		can_collide<t_floor,COL_OTHER>(e,pos,name)
		{}
};

class tiled_floor:
	public t_floor
{
	public:
		tiled_floor(t_engine&,double,double,double);
};
#endif
