#include "main_window.hpp"

#include "objects/objects.hpp"
#include "level-parser/parse.hpp"
#include "register/register.hpp"
#include "engine/flomath.hpp"

#include <cmath>
#include <cstdlib>
#include <gtkmm/main.h>
#include <gdk/gdkkeysyms.h>
#include <sstream>

#define PING (std::cout << "PING!\t"<< __FILE__<<":"<<__LINE__ <<"\n")


template< class T>
std::string tostr(T x){
	std::stringstream buff;
	buff << x;
	return buff.str();
}

template< class T>
T fromstr(std::string x){
	std::stringstream buff(x);
	T temp;
	buff >> temp;
	return temp;
}

struct bird_cam: t_camera, engine_interface{
	bird_cam(t_engine& e):t_camera(e),engine_interface(e,ei_t_key_receiver|ei_moveable){z=10;}
	virtual void key_pressed(unsigned int k){
		switch(k){
		case GDK_Right:x+=.25;break;
		case GDK_Left:x-=.25;break;
		case GDK_Up:y+=.25;break;
		case GDK_Down:y-=.25;break;
		case GDK_minus:z+=.5;break;
		case GDK_equal:
		case GDK_plus:z-=.5;break;
		}
	}
	void operator()(){
		glTranslated(-x,-y,-z);
		glRotated(90,1,0,0);
	}
};

struct ui_updater:engine_interface{
	des_main_window* win;
	ui_updater(t_engine& _e,des_main_window* w):engine_interface(_e,ei_stepable|ei_t_key_receiver),win(w){}
	void step(){
		win->update_ui();
	}
	void key_released(unsigned k){
		switch(k){
		case GDK_space:
		case GDK_Return:win->create_current_object();break;
		case GDK_p: win->create_object_on_position();  
		}
	}
};
// struct test_engine:t_engine{
// 	bird_cam* camera;
// /*	void step(){
// 		draw();
// 	}*/
// };

des_main_window::des_main_window():
	bottom_bar(false,0),
	container(false,0),
	current(0),
	curr_lev(0),
	object_view(8,true)
{
	set_title("The Level Editor");
	
	m_VBox.hide();
	
	remove();

	container  .set_can_focus();
	object_view.set_can_focus();
	
	container.pack_start(m_VBox);
	container.pack_start(object_view);
	
	do_register_ei::t_map& objs=do_register_ei::objs();
	typedef do_register_ei::t_map t_map;
	typedef t_map::iterator iter;
	for(iter i=objs.begin();i!=objs.end();++i){
		cbox.append_text(i->first);
	}
	cbox.show();
	cbox.signal_changed().connect(sigc::mem_fun(*this,&des_main_window::on_object_change));
	object_view.signal_focus().connect(sigc::mem_fun(*this,&des_main_window::update_from_view));
	bottom_bar.pack_start(cbox);

//	x_lab.show();
//	y_lab.show();
//	z_lab.show();
//	bottom_bar.pack_end(z_lab,Gtk::PACK_SHRINK);
//	bottom_bar.pack_end(y_lab,Gtk::PACK_SHRINK);
//	bottom_bar.pack_end(x_lab,Gtk::PACK_SHRINK);
	
/*	object_view.signal_button_press_event().connect(
		sigc::mem_fun(*this, &des_main_window::on_object_view_clicked),false);*/
	
	object_view.signal_focus().connect(
		sigc::mem_fun(*this,&des_main_window::on_object_view_focus));
	
	
	object_view.set_events(Gdk::ALL_EVENTS_MASK);
	
	engine->signal_button_press_event().connect(
		sigc::mem_fun(*this,&des_main_window::on_engine_clicked));
	
	m_VBox.pack_end(bottom_bar,Gtk::PACK_SHRINK);
	m_VBox.show();
	add(container);
	container.show();
}

bool des_main_window::update_from_view(Gtk::DirectionType d)
{
	//TODO
	return true;
}

bool des_main_window::on_engine_clicked(GdkEventButton* )
{
	std::cout<<"PING!\n";
	return true;
}


bool des_main_window::on_object_view_focus(Gtk::DirectionType dir)
{
	std::cout << dir << '\n';
	return true;
}

void des_main_window::show_menu()
{
	object_view.hide();
	bottom_bar.hide();
	main_window::show_menu();
}


void des_main_window::create_current_object()
{
	bird_cam* cam=dynamic_cast<bird_cam*>(engine->camera);
	const std::string& name=cbox.get_active_text();
	if(name==""){
		return;
	}
	engine_interface* it= do_register_ei::objs()[name](*engine,cam->x,cam->z-20,-cam->y);
	curr_lev->objects.push_back(new object(name,cam->x,cam->z-20,-cam->y));
	it->load_textures();
}

void des_main_window::on_object_change()
{
	bird_cam* cam=dynamic_cast<bird_cam*>(engine->camera);
	delete current;
	current=do_register_ei::objs()[cbox.get_active_text()](*engine,cam->x,cam->z-20,-cam->y);
	current->load_textures();
}

void des_main_window::update_ui()
{
	bird_cam* cam=dynamic_cast<bird_cam*>(engine->camera);
	if(current){
		current->x=cam->x;
		current->y=cam->z-20;
		current->z=-cam->y;
	}
	std::stringstream sx;
	std::stringstream sy;
	std::stringstream sz;
	sx<<"x="<<cam->x;
	sy<<"y="<<cam->y;
	sz<<"z="<<cam->z;
	
	x_lab.set_text(sx.str());
	y_lab.set_text(sy.str());
	z_lab.set_text(sz.str());
}

des_main_window::~des_main_window(){
	write_out();
}

void des_main_window::write_out()
{
	if(!curr_lev) return;
	std::cout << "write out changes\n";
	int j=0;
	std::ofstream out(("data/levels/"+curr_lev->fname).c_str());
	out << "LEVEL \"" << curr_lev->name << "\"{\n";
	for(std::deque<object*>::iterator i=curr_lev->objects.begin();i!=curr_lev->objects.end();++j,++i){
		if(j==curr_lev->camera.companion_index+1){
			out <<'}'; 
		}
		out << '\n';
		if(j==curr_lev->camera.companion_index){
			out <<"CAMERA "<<curr_lev->camera.name<<'{'; 
		}
		out << (*i)->name;
		if((*i)->mode==object::CRE) continue;
		out<<'\t' << (*i)->x<<'\t' <<(*i)->y <<'\t'<< (*i)->z;
		if((*i)->mode==object::CRE_3d) continue;
		out<<'\t' << (*i)->rx <<'\t'<<(*i)->ry<<'\t' << (*i)->rz;
		if((*i)->mode==object::CRE_6d) continue;
		out<<'\t' << '"' <<(*i)->param<< '"';

	}
	if(j==curr_lev->camera.companion_index+1){
		out <<"\t}"; 
	}
	out << "\n}\n";
}



void des_main_window::load_level(t_level* l){
	write_out();
	curr_lev=l;
	container.show_all();
	menu.hide();
	bottom_bar.show();
	in_menu = false;
	object_view.clear_items();
	int j=0;
	for(std::deque<object*>::iterator i=l->objects.begin();i!=l->objects.end();++i,j++){
		do_register_ei::t_map::const_iterator p_new_ei=do_register_ei::objs().find((*i)->name);
		if(p_new_ei==do_register_ei::objs().end()){
			std::cerr << "WARNING: object by name '" << (*i)->name <<"' not found\n"; 
			continue;
		} 
		object_view.prepend_text((*i)->name);
		engine_interface* that=(p_new_ei->second)(*engine,(*i)->x,(*i)->y,(*i)->z);//lijpe shit
		object_view.set_text(0,1,tostr((*i)->x));
		object_view.set_text(0,2,tostr((*i)->y));
		object_view.set_text(0,3,tostr((*i)->z));
		switch((*i)->mode){
			case object::CRE_6d:{
				that->rotation = flomath::aal_rot_to_quaternion((*i)->rx,(*i)->ry,(*i)->rz);
				object_view.set_text(0,4,tostr((*i)->rx));
				object_view.set_text(0,5,tostr((*i)->ry));
				object_view.set_text(0,6,tostr((*i)->rz));
				break;
			}
			case object::CRE_6ds:{
				that->rotation = flomath::aal_rot_to_quaternion((*i)->rx,(*i)->ry,(*i)->rz);
				that->construct_param((*i)->param);
				object_view.set_text(0,7,(*i)->param);
				break;
			}
		}
	}
	engine->key_receivs.clear();
	engine->moveables.clear();
	engine->steppables.clear();
	engine->set_camera(new bird_cam(*engine));
	new ui_updater(*engine,this);
	if(engine->texture_load_ready){
		engine->load_textures();
	}
	else{
		engine->texture_load_ready = true;
	}
	object_view.show();
	engine->show();
	engine->run();
}

class object_create_window:
	public Gtk::Window
{
	des_main_window* win;
	Gtk::Entry e1,e2,e3;
	Gtk::Label l1,l2,l3;
	Gtk::HBox hbox;

	
public:	
	object_create_window(des_main_window* _w):
		l1("x: "),l2("y: "),l3("z: ")
	{
		win = _w;
		add(hbox);
		hbox.pack_start(l1);
		hbox.pack_start(e1);
		hbox.pack_start(l2);
		hbox.pack_start(e2);
		hbox.pack_start(l3);
		hbox.pack_start(e3);
		show_all_children();
	}
    
	~object_create_window(){
		std::cout << "Ik ben dood\n";
	}
};


void des_main_window::create_object_on_position(){
	object_create_window* w = new object_create_window(this);
	w->show();
}




