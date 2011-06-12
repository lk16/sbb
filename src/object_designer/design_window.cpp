#include "designer_window.hpp"

namespace design{
	
	class camera: 
		public t_camera,
		public t_key_receiver
	{
		double xr,yr,r;
		
	public:
		camera(t_engine& t):
			t_camera(t),
			t_key_receiver(t),
			xr(0),yr(0),
			r(2)
		{}
		
		void key_pressed(unsigned k){
			switch(k){
				case GDK_Up:
					xr+=.4;break;
				case GDK_Down:
					xr-=.4;break;
				case GDK_Left:
					yr+=.4;break;
				case GDK_Right:
					yr-=.4;break;
			}
		}
		
		void operator()(){
			glTranslated(0,0,-r);
			glRotated(xr,1,0,0);
			glRotated(yr,0,1,0);
		}
	};
}


struct test : public drawable{
	test(t_engine& e):drawable(e){}
	void draw(){
		glBegin(GL_QUADS);
		glColor3b(123,123,123);
		glVertex3d( .1, .1,0);
		glVertex3d( .1,-.1,0);
		glVertex3d(-.1,-.1,0);
		glVertex3d(-.1, .1,0);
		
		glEnd();
	}
};

design_window::design_window():
	engine(*new t_engine)
{
	new design::camera(engine);
	new test(engine);
	m_VBox.add(engine);
	add(m_VBox);
	show_all_children();
}

bool design_window::on_key_press_event(GdkEventKey* k){
	engine.key_press(k->keyval);
	return true;
}

bool design_window::on_key_release_event(GdkEventKey* k){
	engine.key_release(k->keyval);
	return true;
}

bool design_window::on_button_press_event(GdkEventButton b){
}

design_window::~design_window(){
}