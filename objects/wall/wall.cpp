#include "wall.hpp"
#include "register/register.hpp"

SBB_REGISTER_ei(wall);
/*
void wall::draw(){
	glColor3b(0,120,0);
	draw_col_fig();
}*/

wall::wall (t_engine& e,flomath::point p):
	can_collide<wall,COL_PLANE>(e,p,/*flomath::polygon(
		point(0, 4.0, 4.0),
		point(0,-4.0, 4.0),
		point(0,-4.0,-4.0),
		point(0, 4.0,-4.0)
	)*/ "data/wall.obj")
{
	type="wall";
}
