#include <cmath>
#include <cstdlib>
#include <sstream>
#include <gdk/gdkkeysyms.h>

#include <gtkmm/main.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/listviewtext.h>
#include <gtkmm/scrolledwindow.h>

#include "main_sbb/main_window.hpp"
#include "main_sbb/main_menu.hpp"
#include "engine/engine.hpp"
#include "engine/flomath.hpp"
#include "engine/util.hpp"
#include "level-parser/parse.hpp"
#include "objects/objects.hpp"
#include "register/register.hpp"






namespace ldes{
	struct ui_updater:
		engine_interface
	{
		ldes::main_window* win;
		
		ui_updater(ldes::main_window*);
		
		void step();
		void key_released(unsigned);
	};
	
	struct bird_cam: 
		t_camera, 
		engine_interface
	{
		bool shift_down,moved_last_time;
		
		bird_cam();
		
		void operator()();
		virtual void step();
		virtual void key_pressed(unsigned);
		
	};
	
	
	class main_window:
		public sbb::main_window
	{
		friend class ldes::ui_updater;
		
		public:
			main_window();
			virtual ~main_window();

			virtual void load_level(t_level*);
			virtual void show_menu();
			
			virtual bool on_key_press_event(GdkEventKey*);
			
			void reload();
			void delete_selected_object();
			void update_ui();
			void on_object_change();
			
			void create_current_object();
			void write_out();
			void create_object_on_position();
			void update_from_view(/*Gtk::DirectionType d*/);

		protected:
			
			t_level* curr_lev;

			Gtk::HBox bottom_bar,container;
			Gtk::ComboBoxText cbox;
			Gtk::Label x_lab,y_lab,z_lab;
			engine_interface* current;
			Gtk::ListViewText object_view;
			Gtk::ScrolledWindow object_view_scroll;
	};
}