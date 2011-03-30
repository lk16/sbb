#include "cube.hpp"
#include <GL/gl.h>
#include "register/register.hpp"

SBB_REGISTER_ei(cube);

cube::cube(t_engine& e,double _x,double _y,double _z):
	can_collide< cube , COL_OTHER>(e,_x,_y,_z,"data/test.obj")
{}

void cube::draw(){
	glColor3b(123,13,1);
	figure::draw();
}

