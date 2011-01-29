#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <gtkmm/drawingarea.h>
#include <vector>
#include <set>
#include <gtkglmm.h>
#include <fstream>

//#include "dbg/glconfigutil.hpp"

class main_window;
class des_main_window;
class engine_interface;
class t_camera;


class t_engine:
	public Gtk::DrawingArea,
	public Gtk::GL::Widget<t_engine>
{
	friend class main_window;
	friend class des_main_window;

	public:
		t_engine(/*main_window**/);
		virtual ~t_engine();
		void gl_begin();
		void gl_end();
		
		void add_moveable(engine_interface*);
		void remove_moveable(engine_interface*);
		
		void add_drawable(engine_interface*);
		void remove_drawable(engine_interface*);
		
		void add_steppable(engine_interface*);
		void remove_steppable(engine_interface*);
		
		void add_has_texture(engine_interface*);
		void remove_has_texture(engine_interface*);
		
		void add_key_receiver(engine_interface*);
		void remove_key_receiver(engine_interface*);
				
		void set_camera(t_camera*);
		void remove_camera(t_camera*);
		
		///BEWARE
		void key_press(unsigned);
		void key_release(unsigned);
		
	protected:
		std::vector<engine_interface*> drawables;
		std::vector<engine_interface*> key_receivs;
		std::vector<engine_interface*> moveables;
		std::vector<engine_interface*> steppables;
		std::vector<engine_interface*> has_textures;
		t_camera* camera;

		virtual void do_collisions();
		void draw();
		bool step();
		
		virtual void on_realize();
		virtual bool on_configure_event(GdkEventConfigure*);
		virtual bool on_expose_event(GdkEventExpose*);
		virtual bool on_button_press_event(GdkEventButton*);
		virtual bool on_key_press_event(GdkEventKey*);
		virtual bool on_key_release_event(GdkEventKey*);
		
	private:
		void do_steps();
		void do_moves();
		void do_keys();
		void load_textures();
		
		bool texture_load_ready;
		
		std::set<unsigned> keys_down;
//		main_window* win;
		int lockdown; /* necessary to stop the main loop temporarily to clear engine*/
		
		void clear_all();
		void run();
		void stop();
		bool stopped;
		
};

#endif
