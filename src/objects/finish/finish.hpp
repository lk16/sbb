#ifndef FINISH_HPP
#define FINISH_HPP
#include "engine/engine.hpp"
#include "engine/collision_test.hpp"
class finish:
public can_collide<finish,COL_PLANE>
{
	public:
		finish(t_engine& e,flomath::point);
		void draw();
};
#endif