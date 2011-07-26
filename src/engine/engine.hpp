#ifndef SBB_ENGINE_HPP
#define SBB_ENGINE_HPP

#include <gtkmm/drawingarea.h>
#include <vector>
#include <set>
#include <gtkglmm.h>
#include <algorithm>
#include <cstdlib>
#include <GL/gl.h>
#include <GL/glu.h>
#include <gdk/gdkkeysyms.h>
#include "engine_interface.hpp"

#if   defined(DEBUG_LOAD_KEYS) || defined(DEBUG_LOG_KEYS)
#include <fstream>
#endif

namespace sbb{
	class main_window;
}

namespace ldes{
	class main_window;
}

class t_camera;
class engine_interface;
class t_engine;
class screen;

extern bool debug;

struct screen:
	public Gtk::DrawingArea,
	public Gtk::GL::Widget<t_engine>
{
	t_camera* camera;
	void set_camera(t_camera*);
	void remove_camera(t_camera*);
		
	virtual void on_realize();
	virtual bool on_configure_event(GdkEventConfigure*);
	virtual bool on_expose_event(GdkEventExpose*);
	virtual void draw();
	virtual ~screen(){}
};


class t_engine
{
private:
	static t_engine* engine_ptr;

	friend class sbb::main_window;
	friend class ldes::main_window;

	
	template<class T>
	struct list:
		public std::vector<T>
	{
		//friend class t_engine;
		void add(T* t){
			this->push_back(t);
		}
		void remove(T* arg1){
			this->erase(std::remove(this->begin(), this->end(), arg1), this->end());
		}
	};
	
	template<> struct list<screen*>:
		public std::vector<void*>
	{
		void add(screen* s){
			s->signal_configure_event().connect(sigc::mem_fun(t_engine::get(),&t_engine::load_textures));
			this->push_back(s);
		}
	};
public:
		
		t_engine();
		virtual ~t_engine();
		
		///BEWARE
		void key_press(unsigned);
		void key_release(unsigned);
		
		sigc::signal<void> signal_collision;

		typedef list<engine_interface*> engine_interface_list;
		engine_interface_list ei_list[6];

		engine_interface_list &has_textures, &steppables, 
		&drawables, &key_receivs, &moveables, 
		&transparent_drawables;
		
		// returns pointer to t_engine::the_engine
		static t_engine* get();
		
protected:
		
		list<screen*> screens;
		
		void do_collisions();
		void draw();
		bool step();
		
		virtual bool on_button_press_event(GdkEventButton*);
		virtual bool on_key_press_event(GdkEventKey*);
		virtual bool on_key_release_event(GdkEventKey*);
		
		void do_steps();
		void do_moves();
		void do_keys();
		void load_textures();
		
		bool texture_load_ready;
		
		std::set<unsigned> keys_down;
		
		void clear_all();
		void run();
		void stop();
		bool stopped;
		
};

#endif
