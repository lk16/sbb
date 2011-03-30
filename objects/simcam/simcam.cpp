#include "simcam.hpp"
#include "register/register.hpp"
#include <GL/gl.h>

SBB_REGISTER_cam(simcam);
simcam::simcam(t_engine& e,const engine_interface* p):
	t_camera(e),
	mv(p)
{}

void simcam::operator()(){
	using flomath::pi;
	flomath::quaternion rot(mv->rotation);
	rot*=flomath::quaternion(cos(-pi/20),sin(-pi/20),0,0);
	glRotated(-180/pi*2*flomath::acos(rot.a),rot.b,rot.c,rot.d);
	flomath::point q(0,.5,4);
	q.rotate(rot);
	glTranslated(-mv->x-q.x,-mv->y-q.y,-mv->z-q.z);
}
