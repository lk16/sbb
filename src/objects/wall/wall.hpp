#ifndef WALL_HPP
#define WALL_HPP
#include "engine/engine.hpp"
#include "engine/collision_test.hpp"


class wall:
	public can_collide<wall,COL_PLANE>
{
	public:
// 	virtual void draw();
	wall(t_engine&,flomath::point);
};

#endif
