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

	struct engine_interface_list:
		private std::vector<engine_interface*> 
	{
		friend class t_engine;
		void add(engine_interface*);
		void remove(engine_interface*);
		using std::vector<engine_interface*>::clear;
	};
	
	public:
		t_engine(/*main_window**/);
		virtual ~t_engine();
		void gl_begin();
		void gl_end();
		
		void set_camera(t_camera*);
		void remove_camera(t_camera*);
		
		///BEWARE
		void key_press(unsigned);
		void key_release(unsigned);
		
		sigc::signal<void> signal_collision;

		engine_interface_list ei_list[6];

		engine_interface_list &has_textures, &steppables, 
		&drawables, &key_receivs, &moveables, 
		&transparent_drawables;

	protected:

		
		t_camera* camera;

		void do_collisions();
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
//		int lockdown; /* necessary to stop the main loop temporarily to clear engine*/
		
		void clear_all();
		void run();
		void stop();
		bool stopped;
		
};

#endif
