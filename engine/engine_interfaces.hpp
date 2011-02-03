#ifndef ENGINE_INTERFACES_HPP
#define ENGINE_INTERFACES_HPP

#include "flomath.hpp"
#include "engine.hpp"
#include <bitset>

enum{
	ei_has_texture = 1,
	ei_stepable = 2,
	ei_drawable = 4,
	ei_t_key_receiver = 8,
	ei_moveable = 16
}; 

class engine_interface:
	virtual public flomath::point
{
	std::bitset<5> interfaces;
	t_engine* e;

public:
	flomath::vector speed;
	flomath::vector accel;
	flomath::quaternion rotation;
	
	engine_interface(t_engine& _e,unsigned input);
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
	void construct_param(std::string s);
	
	virtual void load_textures(){}
	virtual void step(){}
	virtual void draw(){}
	virtual void key_pressed(unsigned){}
	virtual void key_released(unsigned){}
	
	engine_interface& operator=(const engine_interface& rhs);
};

struct t_camera{
	t_engine* e;
	
	t_camera(t_engine& _e);
//	t_camera(const t_camera& tc);
	virtual ~t_camera();

	
	virtual void operator()()=0;
};

#endif
