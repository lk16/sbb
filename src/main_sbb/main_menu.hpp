#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <gtkmm/box.h>
#include <gtkmm/button.h>

namespace sbb{
	class main_window;
	
	class main_menu:
		public Gtk::VBox
	{
		public:
			main_menu(main_window* m);
			void on_quit();
			void on_start();
			void reset();
			void show();
		protected:
			main_window* win;
			Gtk::Button quit_butt;
			Gtk::Button start_butt;
	};
}


#endif