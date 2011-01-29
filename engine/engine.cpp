#include "engine.hpp"
#include "collision_test.hpp"
//#include "other.hpp"
//#include "main_window.hpp"
//#include "engine_interfaces.hpp"
#include <algorithm>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gdk/gdkkeysyms.h>

//#define DEBUG_LOAD_KEYS

#if defined(DEBUG_LOAD_KEYS) && defined(DEBUG_LOG_KEYS)
#error "you can only load OR store the keypresses, not both"
#endif
#if   defined(DEBUG_LOAD_KEYS)
std::ifstream key_log("key_log");
#elif defined(DEBUG_LOG_KEYS)
std::ofstream key_log("key_log");
#endif

typedef std::vector<engine_interface*>::iterator eip_iter;

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

void t_engine::remove_drawable(engine_interface* arg1) {
	drawables.erase(std::remove(drawables.begin(), drawables.end(), arg1), drawables.end());
}

void t_engine::remove_key_receiver(engine_interface* arg1) {
	key_receivs.erase(std::remove(key_receivs.begin(), key_receivs.end(), arg1), key_receivs.end());
}

void t_engine::remove_moveable(engine_interface* arg1) {
	moveables.erase(std::remove(moveables.begin(), moveables.end(), arg1), moveables.end());
}

void t_engine::remove_steppable(engine_interface* arg1) {
	steppables.erase(std::remove(steppables.begin(), steppables.end(), arg1), steppables.end());
}

void t_engine::do_steps() {
	for (eip_iter i = steppables.begin();i != steppables.end();i++) {
		(*i)->step();
	}
}

void t_engine::add_steppable(engine_interface* arg1) {
	steppables.push_back(arg1);
}

void t_engine::add_moveable(engine_interface*m) {
	moveables.push_back(m);
}

void t_engine::do_moves() {
	for (eip_iter i = moveables.begin();i != moveables.end();i++) {
		(*i)->move();
	}
}

void t_engine::do_collisions() {}

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
	if (!(lockdown||stopped)) {
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
		lockdown(0),
		stopped(true){
	
	set_can_focus();//blablah
	debug = false;
	Glib::RefPtr<Gdk::GL::Config> glconfig;
	// Try double-buffered visual
	glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA    |
	                                   Gdk::GL::MODE_DEPTH   |
	                                   Gdk::GL::MODE_DOUBLE);
	if (!glconfig) {
		std::cerr << "*** Cannot find the double-buffered visual." << std::endl;
		std::cerr << "*** Trying single-buffered visual." << std::endl;

		// Try single-buffered visual
		glconfig = Gdk::GL::Config::create(Gdk::GL::MODE_RGBA   |
		                                   Gdk::GL::MODE_DEPTH);
		if (!glconfig) {
			std::cerr << "*** Cannot find any OpenGL-capable visual." << std::endl;
			std::exit(1);
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


void t_engine::add_key_receiver(engine_interface* key_rec) {
	key_receivs.push_back(key_rec);
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
	gluPerspective(40.0, 1.0, 0.1, 50.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 3.0,
	          0.0, 0.0, 0.0,
	          0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -3.0);

	std::cout << "configure ready" << std::endl;
	if (texture_load_ready) {
		load_textures();
	}
	else {
		texture_load_ready = true;
	}
	draw();
	glwindow->gl_end();

}
void t_engine::add_has_texture(engine_interface* arg1) {
	has_textures.push_back(arg1);
}
void t_engine::remove_has_texture(engine_interface* arg1) {
	has_textures.erase(std::remove(has_textures.begin(), has_textures.end(), arg1), has_textures.end());
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

	// Swap buffers.
	if (glwindow->is_double_buffered())
		glwindow->swap_buffers();
	else
		glFlush();

	glwindow->gl_end();
}

void t_engine::add_drawable(engine_interface* const d) {
	drawables.push_back(d);
}

void t_engine::gl_begin() {
	get_gl_window()->gl_begin(get_gl_context());
}

void t_engine::gl_end() {
	get_gl_window()->gl_end();
}

void t_engine::clear_all() {
	if (lockdown == 0) {
		lockdown = 1;
		return;
	}
	delete camera;
	camera = 0;
	while (!drawables.empty()) {
		delete drawables[0];
	}
	while (!key_receivs.empty()) {
		delete key_receivs[0];
	}
	while (!moveables.empty()) {
		delete moveables[0];
	}
	while (!steppables.empty()) {
		delete steppables[0];
	}
	while (!has_textures.empty()) {
		delete has_textures[0];
	}
	lockdown = 0;//no genocide next step
	std::cout << "cleared engine garbage" << std::endl;
}

