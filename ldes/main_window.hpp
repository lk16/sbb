#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/listviewtext.h>

#include "main/main_window.hpp"
#include "engine/engine.hpp"
#include "main/main_menu.hpp"
#include "level-parser/parse.hpp"

class ui_updater;

class des_main_window:
	public main_window
{
	friend class ui_updater;
	public:
		des_main_window();
		virtual ~des_main_window();

		virtual void load_level(t_level*);
		virtual void show_menu();
		
		void update_ui();
		void on_object_change();
		bool on_object_view_focus(Gtk::DirectionType);
		bool on_object_view_changed();
		
		bool on_engine_clicked(GdkEventButton*);
		
		void create_current_object();
		void write_out();
		void create_object_on_position();
		void update_from_view(/*Gtk::DirectionType d*/);

	protected:
		void on_button_quit_clicked();
		
		t_level* curr_lev;

		Gtk::HBox bottom_bar,container;
		Gtk::ComboBoxText cbox;
		Gtk::Label x_lab,y_lab,z_lab;
		engine_interface* current;
		Gtk::ListViewText object_view;
};
