#include <iostream>
#include <gtkglmm.h>

#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "engine/engine.hpp"
#include "engine/engine_interface.hpp"
#include "engine/engine.hpp"

class design_window:
	public Gtk::Window
{
	
	public:
		design_window();
		virtual ~design_window();
		virtual bool on_key_press_event(GdkEventKey* k);
		virtual bool on_key_release_event(GdkEventKey* k);
		
	protected:
		Gtk::VBox m_VBox;
		t_engine& engine;
};
