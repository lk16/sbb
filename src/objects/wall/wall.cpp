#include "wall.hpp"

SBB_REGISTER_ei(wall);

wall::wall (const flomath::point& p):
	can_collide<wall,COL_PLANE>(p,"data/wall.obj")
{
	type="wall";
}
