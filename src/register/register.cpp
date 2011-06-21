#include "register.hpp"

do_register_ei::t_map& do_register_ei::objs(){
	static t_map* it=new t_map;
	return *it;
}
	
do_register_cam::t_map& do_register_cam::objs(){
	static t_map* it=new t_map;
	return *it;
}
	