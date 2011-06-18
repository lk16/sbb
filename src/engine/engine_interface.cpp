#include "engine_interface.hpp"

void engine_interface::construct_params()
{
	for(std::map<std::string,std::string>::iterator i=params.begin();i!=params.end();i++){
		show_warning("redundant parameter found '" + i->first + " = " + i->second);
	}
}

t_engine* engine_interface::engine()
{
	return t_engine::get();
}


std::string engine_interface::get_param(const std::string& s)
{
	if(params.find(s)==params.end()){
		show_error("Can not find key '" + s);
	}
	std::string tmp = params[s];
	params.erase(params.find(s));
	return tmp;
}

std::string engine_interface::get_param(const std::string& s, const std::string& def)
{
	std::map<std::string,std::string>::iterator i=params.find(s);
	std::string tmp;
	if(i!=params.end()){
		tmp = i->second;
		params.erase(i);
	}
	else{
		return def;
	}
	return tmp;
}

void engine_interface::set_construct_params(const std::string& s)
{
	std::vector<std::string> tmp(1);
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
			show_error("Key '" + key + "' was already used");
		}
		params[key] = val;
	}
	construct_params();
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
	engine()->ei_list[n].add(this);
}

void engine_interface::remove_interface(unsigned char n){
	if(n>4) return;
	if(!interfaces[n]) return;
	interfaces[n]=false;
	engine()->ei_list[n].remove(this);
}

engine_interface::engine_interface(unsigned input=0):
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

t_camera::t_camera(){
	engine()->set_camera(this);
}

t_camera::~t_camera(){
	engine()->remove_camera(this);
}

engine_interface::engine_interface(const engine_interface& rhs){
	for(size_t i=0;i<5;i++){
		if(rhs.interfaces[i]){
			add_interface(i);
		}
	}
	if(interfaces[4]){ //moveable
		*static_cast<flomath::point*>(this)=rhs;
		speed=rhs.speed;
		accel=rhs.accel;
		rotation=rhs.rotation;
	}
}

t_engine* t_camera::engine()
{
	t_engine::get();
}
