#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <map>
#include <string>

#include "engine/engine_interface.hpp"
#include "engine/flomath.hpp"
#include "engine/engine.hpp"

//TODO replace ugly code

template<class T>
t_camera* create(engine_interface* vict){
	return new T(vict);
}

struct do_register_cam{
	typedef std::map<std::string,t_camera*(*)(engine_interface*)> t_map;
	static t_map& objs();
	do_register_cam(t_camera*(*make)(engine_interface*),const char*name){
		objs()[name]=make;
	}
};

#define SBB_REGISTER_cam(t) do_register_cam SBB_REGISTER_##t(create<t>,#t)


///####################################################################///
///#########################seperator##################################///
///####################################################################///


template<class T>
engine_interface* create(double x,double y,double z){
	return new T(flomath::point(x,y,z));
}

struct do_register_ei{
	typedef std::map<std::string,engine_interface*(*)(double,double,double)> t_map;
	static t_map& objs();
	do_register_ei(engine_interface*(*make)(double,double,double ),const char*name){
		objs()[name]=make;
	}
};

#define SBB_REGISTER_ei(t) do_register_ei SBB_REGISTER_##t(create<t>,#t)

#endif