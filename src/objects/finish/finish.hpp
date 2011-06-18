#ifndef FINISH_HPP
#define FINISH_HPP

#include "engine/engine.hpp"
#include "engine/collision_test.hpp"
#include "register/register.hpp"
#include "engine/can_collide.hpp"

class finish:
	public can_collide<finish,COL_PLANE>
{
	public:
		finish(flomath::point);
		void draw();
};
#endif