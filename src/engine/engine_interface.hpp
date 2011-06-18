#ifndef SBB_ENGINE_INTERFACE_HPP
#define SBB_ENGINE_INTERFACE_HPP

#include <bitset>
#include <map>
#include <iostream>

#include "engine.hpp"

enum{
	ei_has_texture = 1,
	ei_stepable = 2,
	ei_drawable = 4,
	ei_t_key_receiver = 8,
	ei_movable = 16
}; 

#include "flomath.hpp"

class t_engine;

class engine_interface:
	virtual public flomath::point
{	
	std::bitset<5> interfaces;

private:
	std::map<std::string,std::string> params;
	
	
public:
	flomath::vector speed,accel;
	flomath::quaternion rotation;
	
	engine_interface(unsigned input);
	engine_interface(const engine_interface& ei);
	virtual ~engine_interface();
	
	void add_interface_from_bitset(unsigned char n);
	void add_interface(unsigned char n);
	void remove_interface(unsigned char n);
	void accel_same_direction(double factor);
	void add_speed(double _dx,double _dy,double _dz);
	void set_speed(double _dx,double _dy,double _dz);
	void accelerate(double _ax,double _ay, double _az);	
	void move();	
	
	std::string get_param(const std::string& s);
	std::string get_param(const std::string& s,const std::string& def);
	void set_construct_params(const std::string& s);
	virtual void construct_params();
	
	virtual void load_textures(){}
	virtual void step(){}
	virtual void draw(){}
	virtual void key_pressed(unsigned){}
	virtual void key_released(unsigned){}
	
	t_engine* engine();
	
	engine_interface& operator=(const engine_interface& rhs);
};

struct t_camera{
		
	t_camera();
	virtual ~t_camera();
	
	
	virtual void operator()()=0;
	
	t_engine* engine();
	
};

#endif
