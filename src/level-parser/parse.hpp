#ifndef LEVEL_PARSE_HPP
#define LEVEL_PARSE_HPP

#include <string>
#include <deque>

struct object{
	std::string name;
	double x,y,z,rx,ry,rz;
	std::string param;
	
	enum{ 
		CRE,
		CRE_s,
		CRE_3d,
		CRE_3ds,
		CRE_6d,
		CRE_6ds
	}mode;
	
	object(std::string s,double a,double b,double c,double d, double e,double f,std::string p):
		name(s),
		mode(CRE_6ds),
		x(a),y(b),z(c),
		rx(d),ry(e),rz(f),
		param(p)
	{}
	
	object(std::string s,double a,double b,double c,double d, double e,double f):
		name(s),
		mode(CRE_6d),
		x(a),y(b),z(c),
		rx(d),ry(e),rz(f)
	{}
	
	object(std::string s,double a,double b,double c):
		name(s),
		mode(CRE_3d),
		x(a),y(b),z(c)
	{}
	
	object(std::string s,double a,double b,double c,std::string p):
		name(s),
		mode(CRE_3ds),
		x(a),y(b),z(c),
		param(p)
	{}
	
	object(std::string s):
		name(s),
		mode(CRE),
		x(0),y(0),z(0)
	{}	
	
	object(std::string s,std::string p):
		name(s),
		mode(CRE_s),
		x(0),y(0),z(0),
		param(p)
	{}	
};

struct camera_diy{
	std::string name;
	size_t companion_index;
};

struct t_level{
	std::string fname,name;
	std::deque<object*> objects;
	camera_diy camera;
};

t_level level_parse(const std::string& s);
#endif