#include "main_window.hpp"

#include "objects/objects.hpp"
#include "level-parser/parse.hpp"
#include "register/register.hpp"

#include <cmath>
#include <gtkmm/main.h>
#include <gdk/gdkkeysyms.h>

main_window::main_window():
	in_menu(true),
	m_VBox(false, 0),
	engine(new t_engine/*(this)*/),
	menu(*new main_menu(this))
{
	set_title("Super Bunny Ball");
	set_reallocate_redraws(true);
	add(m_VBox);
	
	set_size_request(300,300);

	engine->set_size_request(200, 200);
	m_VBox.pack_start(*engine);
	m_VBox.pack_start(menu);
	m_VBox.show();
	menu.show();
}

bool main_window::on_key_press_event(GdkEventKey* k){
	switch((k->keyval!=GDK_Escape)|(in_menu<<1)){
		case 0:show_menu();break;
//		case 1:engine->key_press(k->keyval);break;
		case 2:Gtk::Main::quit();break;
	}
	return Gtk::Window::on_key_press_event (k);
}

bool main_window::on_key_release_event(GdkEventKey* k){
//	if(!in_menu){ engine->key_release(k->keyval); }
	return Gtk::Window::on_key_release_event (k);
}

void main_window::show_menu(){
	engine->hide();
	engine->stop();
	engine->clear_all();
	in_menu=true;
	menu.show();
}


main_window::~main_window(){
	delete engine;
	delete &menu;
}

void main_window::on_button_quit_clicked(){
	Gtk::Main::quit();
}

void main_window::load_level(t_level* l){	
	menu.hide();
	engine->show();
	in_menu = false;
	int j=0;
	for(std::deque<object*>::iterator i=l->objects.begin();i!=l->objects.end();++i,j++){
		do_register_ei::t_map::const_iterator p_new_ei=do_register_ei::objs().find((*i)->name);
		if(p_new_ei==do_register_ei::objs().end()){
			std::cerr << "WARNING: object by name '" << (*i)->name <<"' not found\n"; 
			continue;
		}
		engine_interface* that=(p_new_ei->second)(*engine,(*i)->x,(*i)->y,(*i)->z);//lijpe shit
		switch((*i)->mode){
			case object::CRE_6d:{
				that->rotation = flomath::aal_rot_to_quaternion((*i)->rx,(*i)->ry,(*i)->rz);
				break;
			}
			case object::CRE_6ds:{
				that->rotation = flomath::aal_rot_to_quaternion((*i)->rx,(*i)->ry,(*i)->rz);
				that->construct_param((*i)->param);
				break;
			}
		}
		if(j==l->camera.companion_index){
			do_register_cam::t_map::const_iterator p_new_cam=do_register_cam::objs().find(l->camera.name);
			if(p_new_cam==do_register_cam::objs().end()){
				std::cerr << "WARNING: camera by name '" << l->camera.name <<"' not found\n"
						 "WARNING: not loading level!!\n";
				show_menu();///bug in level, we do need a camera!
				continue;
			}
			(p_new_cam->second)(*engine,that);
		}
	}
	if(engine->texture_load_ready){
		engine->load_textures();
	}
	else{
		engine->texture_load_ready = true;
	}
	engine->run();
}





