#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include <main/main_menu.hpp>
#include <gtk/button.hpp>

class ldes_main_menu:
	public main_menu
{
	Gtk::Button create_level_button;
		
	ldes_main_menu(main_window* m):
		main_menu(m)
	{
		create_current_object.signal_pressed().connect(
			sigc::mem_fun(*this,&main_menu::on_create_level_pressed)
		);
		pack_start(create_level_button,Gtk::PACK_EXPAND_PADDING);
		create_level_button.show();
	}
	
	void on_create_level_pressed(){
		
	}
};


#endif

