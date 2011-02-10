#include "engine_interfaces.hpp"
#include <iostream>
void engine_interface::construct_param(std::string s)
{
	std::cout<<"WARNING: some weirdo sent redundant parameter `"<< s <<"'\n";
}

void engine_interface::add_interface_from_bitset(unsigned char n){
	for(int i=0;i<5;i++){
		if(n&1<<i){
			add_interface(i);
		}
	}
}

void engine_interface::add_interface(unsigned char n){
	if(n>4){
		segfault();
		return;
	}
	if(interfaces[n]) return;
	interfaces[n]=true;
	e->ei_list[n].add(this);
}

void engine_interface::remove_interface(unsigned char n){
	if(n>4) return;
	if(!interfaces[n]) return;
	interfaces[n]=false;
	e->ei_list[n].remove(this);
}

engine_interface::engine_interface(t_engine& _e,unsigned input=0):
e(&_e),
rotation(1,0,0,0)
{
	for(size_t i=0;i<5;i++){
		if(input&1<<i){
			add_interface(i);
		}
	}
}

engine_interface::~engine_interface(){
	for(size_t i=0;i<5;i++){
		remove_interface(i);
	}
}


engine_interface& engine_interface::operator=(const engine_interface& rhs){
	for(size_t i=0;i<5;i++){
		remove_interface(i);
	}
	e=rhs.e;
	for(size_t i=0;i<5;i++){
		if(rhs.interfaces[i]){
			add_interface(i);
		}
	}
	if(interfaces[4]){
		*static_cast<flomath::point*>(this)=rhs;
		speed=rhs.speed;
		accel=rhs.accel;
		rotation=rhs.rotation;
	}
	return *this;
}

void engine_interface::accel_same_direction(double factor){
	this->speed *= factor;
}

void engine_interface::add_speed(double _dx,double _dy,double _dz){
	*this += flomath::point(_dx,_dy,_dz);
}

void engine_interface::set_speed(double _dx,double _dy,double _dz){
	speed.set(_dx,_dy,_dz);
}	

void engine_interface::accelerate(double _ax,double _ay, double _az){
	accel.set(_ax,_ay,_az);
}

void engine_interface::move(){
	speed += accel;
	*static_cast<flomath::point*>(this) += speed;
}

t_camera::t_camera(t_engine& _e):e(&_e){
	e->set_camera(this);
}

t_camera::~t_camera(){
	e->remove_camera(this);
}

engine_interface::engine_interface(const engine_interface& rhs){
	e=rhs.e;
	for(size_t i=0;i<5;i++){
		if(rhs.interfaces[i]){
			add_interface(i);
		}
	}
	if(interfaces[4]){
		*static_cast<flomath::point*>(this)=rhs;
		speed=rhs.speed;
		accel=rhs.accel;
		rotation=rhs.rotation;
	}
}
/*
t_camera::t_camera(const t_camera& tc){
	e = tc.e;
	e->set_camera(this);
}*/