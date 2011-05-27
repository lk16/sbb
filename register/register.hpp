#ifndef REGISTER_HPP
#define REGISTER_HPP
#include <map>
#include <string>

#include "engine/engine_interfaces.hpp"
#include "engine/flomath.hpp"

class t_engine;
template<class T>
t_camera* create(t_engine& e,engine_interface* vict){
	return new T(e,vict);
}

struct do_register_cam{
	typedef std::map<std::string,t_camera*(*)(t_engine&,engine_interface*)> t_map;
	static t_map& objs();
	do_register_cam(t_camera*(*make)(t_engine&,engine_interface*),const char*name){
		objs()[name]=make;
	}
};

#define SBB_REGISTER_cam(t) do_register_cam SBB_REGISTER_##t(create<t>,#t)


///####################################################################///
///#########################seperator##################################///
///####################################################################///


template<class T>
engine_interface* create(t_engine& e,double x,double y,double z){
	return new T(e,flomath::point(x,y,z));
}

struct do_register_ei{
	typedef std::map<std::string,engine_interface*(*)(t_engine&,double,double,double)> t_map;
	static t_map& objs();
	do_register_ei(engine_interface*(*make)(t_engine&,double,double,double ),const char*name){
		objs()[name]=make;
	}
};

#define SBB_REGISTER_ei(t) do_register_ei SBB_REGISTER_##t(create<t>,#t)


#endif