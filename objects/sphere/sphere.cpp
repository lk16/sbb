#include <gdk/gdkkeysyms.h>
#include <algorithm>

#include "sphere.hpp"
#include <register/register.hpp>
#include <engine/collision_test.hpp>
#include <objects/floor/floor.hpp>
#include <objects/wall/wall.hpp>
#include <objects/finish/finish.hpp>
#include "../../engine/util.hpp"


SBB_REGISTER_ei(player_ball);

void player_ball::key_pressed(unsigned k) {
	using flomath::pi;
	using flomath::vector;
	using std::sin;
	using std::cos;
	switch (k) {
		case GDK_Left:
			rotation *= flomath::quaternion(cos(pi/90),vector(0,1,0)*sin(pi/90));
			break;
		case GDK_Right:
			rotation *= flomath::quaternion(cos(-pi/90),vector(0,1,0)*sin(-pi/90));
			break;
		case GDK_Up:
		case GDK_Down:
			flomath::vector acc(0,0,-0.03);
			acc.rotate(rotation);
			if(k==GDK_Up) 
				speed+=acc;
			else
				speed-=acc;
			speed*=0.95;
	}
}

void player_ball::step() {
//	accel_same_direction(.99);
	if (y < -5) {
		x = 0;
		y = 1;
		z = 0;
		set_speed(0, 0, 0);
	}
	time++;
	if (prevcol == DOB_TRUE) {
		prevcol = DOB_TRY_FALSE;
	}
	else if (prevcol == DOB_TRY_FALSE) {
		prevcol = DOB_FALSE;
	}
	accel_same_direction(0.999);
}
struct dist_to_p_cmp { flomath::point p1;
	dist_to_p_cmp(flomath::point p0):p1(p0){}
	bool operator()(const flomath::point& r,const flomath::point& l){ 
		return flomath::distance(r,p1)<flomath::distance(l,p1);
}};

struct p_same_normal_cmp{ 
	flomath::point p1;
	p_same_normal_cmp(flomath::point p0):p1(p0){}
	bool operator()(const flomath::polygon& o){
		return flomath::plane(o).normal.normalize() == p1;
	}
};

//TODO FIXIT
void player_ball::collide(collision_data<t_floor>& f) {
	flomath::plane p(*f.p_other);
	flomath::vector normal = p.normal / p.normal.length();
	normal.rotate(f.other->rotation);
	flomath::vector vovernormal = normal * flomath::dotproduct(speed, normal);
	*this -= vovernormal;
	if (::collide(*this, f)) {//WTF? blijkbaar kan hij de overload wel vinden als je expliciet zegt dat het global is, maar anders niet..
		*this+=vovernormal;
		flomath::polygon top(*std::find_if(f.other->polly.begin(),f.other->polly.end(),p_same_normal_cmp(flomath::vector(0, 1, 0))));
		flomath::point p_wrpt_f=*this-f;
		p_wrpt_f.rotate(f.other->rotation);
		std::vector<flomath::point> edge_list(top.p);
		assert(top.p.size() >= 2);
		std::nth_element(edge_list.begin(),edge_list.begin()+1,edge_list.end(),dist_to_p_cmp(p_wrpt_f));

		flomath::plane q(edge_list[0],edge_list[1], flomath::point(0, 4, 0));//BUG kludge
		normal = q.normal; 
		vovernormal = normal * flomath::dotproduct(speed, normal)/normal.length_sqr();
		*this -= vovernormal;
		speed -= vovernormal * 2 * .3;
	}
	else{
		speed -= vovernormal * 2 * .7;
	}
}

void player_ball::collide(base_plane& f) {
	flomath::vector normal = f.normal / f.normal.length();
	flomath::vector vovernormal = normal * flomath::dotproduct(speed, normal);
	*this -=
	vovernormal;
	speed -= vovernormal * 2 * .7;
}

void player_ball::collide(finish) {
	if (prevcol == DOB_TRUE) return;
	if (prevcol == DOB_TRY_FALSE) {
		prevcol = DOB_TRUE;
		return;
	}
	std::cout << "your time was :" << time << '\n';
	time = 0;
	prevcol = DOB_TRUE;
}

void player_ball::draw() {
	using namespace std;
	const int prec = 30;
	glColor4f(0, 0, 1, .6);
	glPushMatrix();
	glTranslated(x, y, z);
	glRotated(2*180/flomath::pi*flomath::acos(rotation.a), rotation.b, rotation.c, rotation.d);

	glBegin(GL_TRIANGLES);
	for (size_t i = 1;i < prec;i++) {
		glVertex3d(r*std::cos(i*2*flomath::pi / prec), r*std::sin(i*2*flomath::pi / prec), 0);
		glVertex3d(r*std::cos((i - 1)*2*flomath::pi / prec), r*std::sin((i - 1)*2*flomath::pi / prec), 0);
		glVertex3d(0, 0, 0);
	}
	glVertex3d(r*std::cos((prec - 1)*2*flomath::pi / prec), r*std::sin((prec - 1)*2*flomath::pi / prec), 0);
	glVertex3d(r, 0, 0);
	glVertex3d(0, 0, 0);

	glEnd();
	glPopMatrix();
}

player_ball::player_ball(t_engine& e, double _x, double _y, double _z):
		can_collide< player_ball , COL_SPHERE>(e, _x, _y, _z, .5, false),
		prevcol(DOB_FALSE),
		time(0) {
	add_interface_from_bitset(ei_moveable | ei_t_key_receiver | ei_stepable );
	e.transparent_drawables.add(this);
	e.drawables.remove(this);
	accel.y = -0.010;
	type = "t_sphere";
	e.signal_collision.connect(&::collision<player_ball, t_floor>);
	e.signal_collision.connect(&::collision<player_ball, wall>);
	e.signal_collision.connect(&::collision<player_ball, finish>);
		
}
player_ball::~player_ball(){
	e->transparent_drawables.remove(this);
}