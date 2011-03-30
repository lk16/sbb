#include "finish.hpp"
#include "register/register.hpp"

SBB_REGISTER_ei(finish);

finish::finish(t_engine& e,double _x,double _y,double _z):
	flomath::point(_x,_y,_z),
	can_collide< finish , COL_PLANE>(e,_x,_y,_z,flomath::polygon(
		point(-1,-1,0),
		point(-1, 1,0),
		point( 1, 1,0),
		point( 1,-1,0)
	))
{}
void finish::draw(){
	glColor3b(123,0,123);
	//		can_collide<finish>::draw();
	glPushMatrix();
	glTranslated(x,y,z);
	glBegin(GL_QUADS);
	glVertex3d(1.2,-1,0);
	glVertex3d(1.0,-1,0);
	glVertex3d(1.0, 1,0);
	glVertex3d(1.2, 1,0);
	
	glVertex3d(-1.2,-1,0);
	glVertex3d(-1.0,-1,0);
	glVertex3d(-1.0, 1,0);
	glVertex3d(-1.2, 1,0);
	glEnd();
	glPopMatrix();
}
