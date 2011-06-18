#ifndef WALL_HPP
#define WALL_HPP

#include "engine/engine.hpp"
#include "engine/collision_test.hpp"
#include "engine/can_collide.hpp"
#include "register/register.hpp"

class wall:
	public can_collide<wall,COL_PLANE>
{
	public:
	wall(const flomath::point&);
};

#endif
