#include "main_window.hpp"


namespace ldes{

	bird_cam::bird_cam():
		t_camera(),
		engine_interface(ei_t_key_receiver|ei_movable|ei_stepable)
	{
		z = 10;
		moved_last_time = false;
		shift_down = false;
	}

	void bird_cam::step(){
		if(!shift_down || !moved_last_time){
			*this+=speed;
		}
		moved_last_time = (speed!=flomath::point(0,0,0));
		speed=flomath::point(0,0,0);
		shift_down=false;
	}
	
	void bird_cam::key_pressed(unsigned int k){
		switch(k){
		case GDK_Right:
			speed.x=.125;
			break;
		case GDK_Left:
			speed.x=-.125;
			break;
		case GDK_Up:
			speed.y=.125;
			break;
		case GDK_Down:
			speed.y=-.125;
			break;
		case GDK_minus:
		case GDK_underscore:	
			speed.z=.25;
			break;
		case GDK_equal:
		case GDK_plus:
			speed.z=-.25;
			break;
		case GDK_Shift_L:
		case GDK_Shift_R:
			shift_down=true;
			break;
		}
	}
	
	void bird_cam::operator()(){
		glTranslated(-x,-y,-z);
		glRotated(90,1,0,0);
	}

	ui_updater::ui_updater(ldes::main_window* w):
		engine_interface(ei_stepable|ei_t_key_receiver),
		win(w)
	{}
	
	void ui_updater::step(){
		win->update_ui();
	}
	
	void ui_updater::key_released(unsigned k){
		switch(k){
		case GDK_space:
		case GDK_Return:win->create_current_object();break;
		}
	}
	



	main_window::main_window():
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

		object_view.set_column_title(0,"name");
		object_view.set_column_title(1,"x");
		object_view.set_column_title(2,"y");
		object_view.set_column_title(3,"z");
		object_view.set_column_title(4,"rx");
		object_view.set_column_title(5,"ry");
		object_view.set_column_title(6,"rz");
		object_view.set_column_title(7,"param");
		
		object_view_scroll.add(object_view);
		container.pack_start(object_view_scroll);
		
		do_register_ei::t_map& objs=do_register_ei::objs();
		typedef do_register_ei::t_map t_map;
		typedef t_map::iterator iter;
		for(iter i=objs.begin();i!=objs.end();++i){
			cbox.append_text(i->first);
		}
		cbox.show();
		cbox.signal_changed().connect(sigc::mem_fun(*this,&main_window::on_object_change));
		engine()->signal_grab_focus().connect(sigc::mem_fun(*this,&main_window::update_from_view));
		bottom_bar.pack_start(cbox);
		
		object_view.set_events(Gdk::ALL_EVENTS_MASK);
		
		m_VBox.pack_end(bottom_bar,Gtk::PACK_SHRINK);
		m_VBox.show();
		add(container);
		container.show();
	}

	void main_window::reload()
	{
		flomath::point pos = *dynamic_cast<bird_cam*>(engine()->camera);
		
		object_view.clear_items();
		engine()->clear_all();
		current=0;
		engine()->stop();
		load_level(curr_lev);
		*static_cast<flomath::point*>(dynamic_cast<bird_cam*>(engine()->camera))=pos; // WTF is this ?!?
		engine()->run();
		on_object_change();
	}



	bool main_window::on_key_press_event(GdkEventKey* k)
	{
		if(k->keyval==GDK_Delete){
			delete_selected_object();
		}
		return main_window::on_key_press_event(k);
	}


	void main_window::delete_selected_object()
	{
		std::vector<int> list= object_view.get_selected();
		for(std::vector<int>::iterator i=list.begin(),e=list.end();i!=e;i++){
			std::cout << *i << '\n';
			delete curr_lev->objects[object_view.size()-*i-1];
			curr_lev->objects.erase(curr_lev->objects.begin()+object_view.size()-*i-1);
		}
		reload();   
	}

	void main_window::update_from_view(/*Gtk::DirectionType d*/)
	{
		curr_lev->objects.clear();
		//TODO repair HACK: geen rekening gehouden met camera:companion_index
		for(unsigned i=object_view.size()-1;i<object_view.size();i--){
			if(object_view.get_text(i,7)!=""){
				curr_lev->objects.push_back(new object(
					object_view.get_text(i,0),
					fromstr<double>(object_view.get_text(i,1)),
					fromstr<double>(object_view.get_text(i,2)),
					fromstr<double>(object_view.get_text(i,3)),
					fromstr<double>(object_view.get_text(i,4)),
					fromstr<double>(object_view.get_text(i,5)),
					fromstr<double>(object_view.get_text(i,6)),
					object_view.get_text(i,7)
				));
				continue;
			}
			if(object_view.get_text(i,4)!=""
				||object_view.get_text(i,5)!=""
				||object_view.get_text(i,6)!=""
			){
				curr_lev->objects.push_back(new object(
					object_view.get_text(i,0),
					fromstr<double>(object_view.get_text(i,1)),
					fromstr<double>(object_view.get_text(i,2)),
					fromstr<double>(object_view.get_text(i,3)),
					fromstr<double>(object_view.get_text(i,4)),
					fromstr<double>(object_view.get_text(i,5)),
					fromstr<double>(object_view.get_text(i,6))
				));
				continue;
			}
			if(object_view.get_text(i,1)!=""
				||object_view.get_text(i,2)!=""
				||object_view.get_text(i,3)!=""
			){
				curr_lev->objects.push_back(new object(
					object_view.get_text(i,0),
					fromstr<double>(object_view.get_text(i,1)),
					fromstr<double>(object_view.get_text(i,2)),
					fromstr<double>(object_view.get_text(i,3))
				));
				continue;
			}
			curr_lev->objects.push_back(new object(object_view.get_text(i,0)));
		}
		reload();
	}

	void main_window::show_menu()
	{
		current=0;
		object_view_scroll.hide();
		bottom_bar.hide();
		main_window::show_menu();
	}


	void main_window::create_current_object()
	{
		bird_cam* cam=dynamic_cast<bird_cam*>(engine()->camera);
		const std::string& name=cbox.get_active_text();
		if(name==""){
			return;
		}
		curr_lev->objects.push_back(new object(name,cam->x,cam->z-20,-cam->y));
		reload();
	}

	void main_window::on_object_change()
	{
		bird_cam* cam=dynamic_cast<bird_cam*>(engine()->camera);
		delete current;
		if(cbox.get_active_text()=="") return;
		current=do_register_ei::objs()[cbox.get_active_text()](cam->x,cam->z-20,-cam->y);
		current->load_textures();
	}

	void main_window::update_ui()
	{
		bird_cam* cam=dynamic_cast<bird_cam*>(engine()->camera);
		if(current){
			current->x=cam->x;
			current->y=cam->z-20;
			current->z=-cam->y;
		}
	}

	main_window::~main_window(){
		write_out();
	}

	void main_window::write_out()
	{
		if(!curr_lev) return;
		int j=0;
		std::ofstream out(curr_lev->fname.c_str());
		out << "LEVEL \"" << curr_lev->name << "\"{";
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
		std::cout << "write out changes\n";
	}



	void main_window::load_level(t_level* l){
		using flomath::deg2rad;
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
				show_warning("object by name '" + (*i)->name  + "' not found"); 
				continue;
			} 
			object_view.prepend_text((*i)->name);
			engine_interface* that=(p_new_ei->second)((*i)->x,(*i)->y,(*i)->z);
			object_view.set_text(0,1,tostr((*i)->x));
			object_view.set_text(0,2,tostr((*i)->y));
			object_view.set_text(0,3,tostr((*i)->z));
			switch((*i)->mode){
				case object::CRE_s:
				case object::CRE_3ds:
					that->set_construct_params((*i)->param);
					object_view.set_text(0,7,(*i)->param);
					break;
				case object::CRE_6d:{
					that->rotation = flomath::aal_rot_to_quaternion(deg2rad((*i)->rx),deg2rad((*i)->ry),deg2rad((*i)->rz));
					object_view.set_text(0,4,tostr((*i)->rx));
					object_view.set_text(0,5,tostr((*i)->ry));
					object_view.set_text(0,6,tostr((*i)->rz));
					break;
				}
				case object::CRE_6ds:{
					object_view.set_text(0,4,tostr((*i)->rx));
					object_view.set_text(0,5,tostr((*i)->ry));
					object_view.set_text(0,6,tostr((*i)->rz));
					that->rotation = flomath::aal_rot_to_quaternion(deg2rad((*i)->rx),deg2rad((*i)->ry),deg2rad((*i)->rz));
					that->set_construct_params((*i)->param);
					object_view.set_text(0,7,(*i)->param);
					break;
				}
			}
		}
		engine()->key_receivs.clear();
		engine()->moveables.clear();
		engine()->steppables.clear();
		engine()->set_camera(new bird_cam());
		new ui_updater(this);
		if(engine()->texture_load_ready){
			engine()->load_textures();
		}
		else{
			engine()->texture_load_ready = true;
		}
		object_view_scroll.show();
		engine()->show();
		engine()->run();
	}

	
}
