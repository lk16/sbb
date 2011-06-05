#include <algorithm>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gdk/gdkkeysyms.h>

//#define DEBUG_LOAD_KEYS

#include "engine.hpp"
#include "util.hpp"
#include "collision_test.hpp"



#if defined(DEBUG_LOAD_KEYS) && defined(DEBUG_LOG_KEYS)
#error "you can only load OR store the keypresses, not both"
#endif
#if   defined(DEBUG_LOAD_KEYS)
std::ifstream key_log("key_log");
#elif defined(DEBUG_LOG_KEYS)
std::ofstream key_log("key_log");
#endif

typedef std::vector<engine_interface*>::iterator eip_iter;

void t_engine::engine_interface_list::add(engine_interface* arg1)
{
	push_back(arg1);
}

void t_engine::engine_interface_list::remove(engine_interface* arg1)
{
	erase(std::remove(begin(), end(), arg1), end());
}


void t_engine::remove_camera(t_camera* arg1) {
	if (camera == arg1) {
		camera = 0;
	}
}

void t_engine::run()
{
	stopped=false;
	Glib::signal_timeout().connect(sigc::mem_fun(*this, &t_engine::step), 50);
}

void t_engine::stop()
{
	stopped=true;
}

void t_engine::do_steps() {
	for (eip_iter i = steppables.begin();i != steppables.end();i++) {
		(*i)->step();
	}
}

void t_engine::do_moves() {
	for (eip_iter i = moveables.begin();i != moveables.end();i++) {
		(*i)->move();
	}
}

void t_engine::do_collisions() {
	signal_collision.emit();
}

#ifdef DEBUG_LOG_KEYS
void t_engine::do_keys() {
	const unsigned zero = 0;
	for (std::set<unsigned>::iterator i = keys_down.begin();i != keys_down.end();i++) {
		for (eip_iter j = key_receivs.begin();j != key_receivs.end();j++) {
			(*j)->key_pressed(*i);
		}
		key_log.write(reinterpret_cast<const char*>(&*i), sizeof(unsigned));
	}
	key_log.write(reinterpret_cast<const char*>(&zero), sizeof(unsigned));
}
#elif defined(DEBUG_LOAD_KEYS)
void t_engine::do_keys() {
	unsigned k;
	key_log.read(reinterpret_cast<char*>(&k), sizeof(unsigned));
	while (key_log&&k) {
		for (eip_iter j = key_receivs.begin();j != key_receivs.end();j++) {
			(*j)->key_pressed(k);
		}
		key_log.read(reinterpret_cast<char*>(&k), sizeof(unsigned));
	}
}
#else
void t_engine::do_keys() {
	for (std::set<unsigned>::iterator i = keys_down.begin();i != keys_down.end();i++) {
		for (eip_iter j = key_receivs.begin();j != key_receivs.end();j++) {
			(*j)->key_pressed(*i);
		}
	}
}
#endif
bool t_engine::step() {
	if (!(/*lockdown||*/stopped)) {
		if (!debug) {
			do_keys();
			do_steps();
			do_moves();
			do_collisions();
		}
		draw();
	}
	else {
		clear_all();
	}
	return !stopped;
}

void t_engine::key_press(unsigned int k) {
#ifndef DEBUG_LOAD_KEYS
	keys_down.insert(k);
	if (k == GDK_Control_L) {
		debug = true;
	}
#endif
}

void t_engine::key_release(unsigned int k) {
#ifndef DEBUG_LOAD_KEYS
	if (k == GDK_Control_L) {
		debug = false;
	}
	keys_down.erase(k);
	for (eip_iter i = key_receivs.begin();i != key_receivs.end();i++) {
		(*i)->key_released(k);
	}
	draw();
#endif
}

bool t_engine::on_expose_event(GdkEventExpose* event) {
	draw();
	return Gtk::Widget::on_expose_event(event);

}

void t_engine::set_camera(t_camera* new_camera) {
	camera = new_camera;
}


t_engine::t_engine():
		camera(0),
		texture_load_ready(false),
//		lockdown(0),
		stopped(true),
		has_textures(ei_list[0]), 
		steppables(ei_list[1]), 
		drawables(ei_list[2]), 
		key_receivs(ei_list[3]), 
		moveables(ei_list[4]), 
		transparent_drawables(ei_list[5])
		{
	
	set_can_focus();
	debug = false;
	Glib::RefPtr<Gdk::GL::Config> glconfig;
	// Try double-buffered visual
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA    |
	                                   Gdk::GL::MODE_DEPTH   |
	                                   Gdk::GL::MODE_DOUBLE);
	if (!glconfig) {
		show_warning("Cannot find the double-buffered visual.");
		show_warning("Trying single-buffered visual.");

		// Try single-buffered visual
		glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA   |
		                                   Gdk::GL::MODE_DEPTH);
		if (!glconfig) {
			show_error("*** Cannot find any OpenGL-capable visual.");
		}
	}

	//GLConfigUtil::examine_gl_attrib(glconfig);

	set_gl_capability(glconfig);

	set_events(Gdk::ALL_EVENTS_MASK);
}

t_engine::~t_engine() {}

bool t_engine::on_button_press_event(GdkEventButton*){
	grab_focus();
}

bool t_engine::on_key_press_event(GdkEventKey* k)
{
	key_press(k->keyval);
}

bool t_engine::on_key_release_event(GdkEventKey* k)
{
	key_release(k->keyval);
}

void t_engine::on_realize() {
	Gtk::DrawingArea::on_realize();

	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	if (!glwindow->gl_begin(get_gl_context()))
		return;


	/*  static GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};
	static GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);*/
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping ( NEW )
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

	glViewport(0, 0, get_width(), get_height());

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 3.0,
	          0.0, 0.0, 0.0,
	          0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -3.0);

	draw();
	glwindow->gl_end();

}

void t_engine::load_textures() {
	for (eip_iter i = has_textures.begin();i != has_textures.end();i++) {
		(*i)->load_textures();
	}
}

bool t_engine::on_configure_event(GdkEventConfigure* event) {
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	if (!glwindow->gl_begin(get_gl_context()))
		return false;

	glViewport(0, 0, get_width(), get_height());
	load_textures();
//	grab_focus();
	
	glwindow->gl_end();

	return true;
}

void t_engine::draw() {
	//
	// Get GL::Window.
	//
	if (camera == 0) return;
	Glib::RefPtr<Gdk::GL::Window> glwindow = get_gl_window();

	//
	// GL calls.
	//

	// *** OpenGL BEGIN ***
	if (!glwindow->gl_begin(get_gl_context()))
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	(*camera)();
	for (eip_iter i = drawables.begin();i != drawables.end();i++) {
		(*i)->draw();
	}

	for (eip_iter i = transparent_drawables.begin();i != transparent_drawables.end();i++) {
		(*i)->draw();
	}
	
	// Swap buffers.
	if (glwindow->is_double_buffered())
		glwindow->swap_buffers();
	else
		glFlush();

	glwindow->gl_end();
}

void t_engine::gl_begin() {
	get_gl_window()->gl_begin(get_gl_context());
}

void t_engine::gl_end() {
	get_gl_window()->gl_end();
}

void t_engine::clear_all() {
	delete camera;
	camera = 0;
	for(int i=sizeof(ei_list)/sizeof(ei_list[0])-1;i>=0;i--){
		while (!ei_list[i].empty()) {
			delete ei_list[i][0];
		}
	}
	std::cout << "cleared engine garbage" << std::endl;
}

