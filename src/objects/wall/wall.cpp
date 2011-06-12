#include "wall.hpp"

SBB_REGISTER_ei(wall);

wall::wall (t_engine& e,const flomath::point& p):
	can_collide<wall,COL_PLANE>(e,p,"data/wall.obj")
{
	type="wall";
}
