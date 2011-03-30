//#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <gtkmm/main.h>
#include <boost/bind.hpp>
#include "main_menu.hpp"
#include "main_window.hpp"

#include "level-parser/parse.hpp"

main_menu::main_menu(main_window* m):
	win(m),
	start_butt("start"),
	quit_butt("quit")
{
	start_butt.signal_pressed().connect(sigc::mem_fun(*this,&main_menu::on_start));
	quit_butt.signal_pressed().connect(sigc::mem_fun(*this,&main_menu::on_quit));
	
	pack_start(start_butt,Gtk::PACK_EXPAND_PADDING);
	pack_start(quit_butt,Gtk::PACK_EXPAND_PADDING);
	
	quit_butt.show();
	start_butt.show();
	
	typedef boost::filesystem::directory_iterator dir_iter;
	
	for(dir_iter i("data/levels");i!=dir_iter();i++){
		t_level* level=new t_level(level_parse(i->path().file_string()));
		Gtk::Button* b=new Gtk::Button(level->name);
		b->signal_pressed().connect(boost::bind(&main_window::load_level,win,level));
		b->signal_pressed().connect(boost::bind(&main_menu::reset,this));
		pack_start(*b);
	}
}

void main_menu::show(){
	hide_all();
	quit_butt.show_all();
	start_butt.show_all();
	Gtk::VBox::show();
}

void main_menu::reset(){
	hide_all();
	quit_butt.show_all();
	start_butt.show_all();
}

void main_menu::on_quit(){
	Gtk::Main::quit();
}

void main_menu::on_start(){
	show_all();
	quit_butt.hide();
	start_butt.hide();
}


