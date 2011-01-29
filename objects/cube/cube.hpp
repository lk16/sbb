#ifndef CUBE_HPP
#define CUBE_HPP
#include "engine/engine.hpp"
#include "engine/collision_test.hpp"

class cube:
	public can_collide<cube,COL_OTHER>
{
	public:
		cube(t_engine&,double,double,double);
		virtual void draw();
};
#endif
