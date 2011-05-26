#include "engine_interfaces.hpp"
#include <iostream>
void engine_interface::construct_params()
{
	for(std::map<std::string,std::string>::iterator i=params.begin();i!=params.end();i++){
		std::cout<<"WARNING: redundant parameter found '"<< i->first << " = " << i->second << "'\n";
	}
}

std::string engine_interface::get_param(const std::string& s)
{
	if(params.find(s)==params.end()){
		std::cout << "Can not find key '" << s << "'\n";
		std::exit(23);
	}
	return params[s];
}

std::string get_param(const std::string& s,const std::string& def){
	return params.find(s)==params.end() ? def : param[s];
	
}

void engine_interface::set_construct_params(const std::string& s)
{
	std::vector<std::string> tmp;
	for(unsigned i=0;i<s.size();i++){
		char c = s[i];
		switch(c){
			case ';':
				tmp.push_back("");
				break;
			default: 
				tmp.back() += c;
				break;
			
		}
	}
	for(unsigned i=0;i<tmp.size();i++){
		std::string cur = tmp[i];
		unsigned pos = cur.find('=');
		std::string key,val;
		
		key = cur.substr(0,pos);
		val = cur.substr(pos+1);
		
		if(params.find(key)!=params.end()){
			std::cout << "Key '" << key << "' was already used\n";
			std::exit(1);
		}
		params[key] = val;
	}
	
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
	rotation(1,0,0,0),
	speed(0,0,0),
	accel(0,0,0)
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