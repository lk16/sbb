#ifndef WALL_HPP
#define WALL_HPP

#include "engine/engine.hpp"
#include "register/register.hpp"
#include "engine/collision_test.hpp"

class wall:
	public can_collide<wall,COL_PLANE>
{
	public:
	wall(t_engine&,const flomath::point&);
};

#endif
