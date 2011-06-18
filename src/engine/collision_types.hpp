#ifndef SBB_COLLISION_TYPES_HPP
#define SBB_COLLISION_TYPES_HPP

#include "figure.hpp"
#include "flomath.hpp"

class base_sphere:
	public figure
{
protected:
	double r;
	
public:
	base_sphere(const flomath::point& pos,double _r,bool gentri);	

	void generate_triangle(unsigned precision);
	double radius() const;
};

struct base_plane :
	public figure,
	public flomath::plane
{
	base_plane(const flomath::point& pos, const flomath::polygon& p);
	base_plane(const flomath::point& pos, const std::string& s);
	
	virtual void load_textures();
};

#endif