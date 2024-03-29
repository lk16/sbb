#include <cmath>

#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gdk/gdkkeysyms.h>

#include "objects/objects.hpp"
#include "level-parser/parse.hpp"
#include "register/register.hpp"
#include "engine/engine.hpp"
#include "main_menu.hpp"

namespace sbb{
	class main_window:
		public Gtk::Window
	{
		public:
			main_window();
			virtual ~main_window();
			virtual bool on_key_press_event(GdkEventKey* k);
			virtual bool on_key_release_event(GdkEventKey* k);
			virtual void load_level(t_level*);
			virtual void show_menu();


		protected:
			t_engine* e;
			bool in_menu;
			Gtk::VBox m_VBox;
			Gtk::Button m_ButtonQuit;
			main_menu& menu;
			
			void on_button_quit_clicked();
			t_engine* engine();
	};
}