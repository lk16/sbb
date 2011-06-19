#ifndef SBB_CAN_COLLIDE_HPP
#define SBB_CAN_COLLIDE_HPP

#include "flomath.hpp"
#include "engine.hpp"
#include "collision_types.hpp"

///T::list will be published and accounted as a std::vector< T* > of all the instances of T 

template <class T>
struct listable{
	static std::vector<T*> list;
	listable(){ list.push_back(static_cast<T*>(this)); }
	~listable(){ list.erase(std::remove(list.begin(),list.end(),static_cast<T*>(this)),list.end()); }
};

template <class T>
std::vector<T*> listable<T>::list;


enum{
	COL_SPHERE,
	COL_PLANE,
	COL_OTHER
};

///inherit this if you want to be able to collide
/*BUG een deel van de code is nog afhankelijk van de impliciete inherit van figure hier
 *in principe moet hier flomath::base_figure komen te staan..
 */
template<class T,unsigned flag/*=COL_OTHER*/> struct can_collide;


template<class T>
struct can_collide<T,COL_SPHERE>:
	public base_sphere,
	public listable<T>
{
	can_collide(double _x,double _y,double _z,double r,bool gentri=true):
		base_sphere(flomath::point(_x,_y,_z),r,gentri)
	{}
	
	can_collide(const flomath::point& pos,double r,bool gentri=true):
		base_sphere(pos,r,gentri)
	{}
};

template<class T>
struct can_collide<T,COL_PLANE>:
	public base_plane,
	public listable<T>
{
	can_collide(double _x,double _y,double _z,const flomath::polygon& pol):
		base_plane(flomath::point(_x,_y,_z),pol)
	{}
	
	can_collide(const flomath::point& pos,const flomath::polygon& pol):
		base_plane(pos,pol)
	{}
	
	can_collide(double _x,double _y,double _z,const std::string& s):
		base_plane(flomath::point(_x,_y,_z),s)
	{}
	
	can_collide(flomath::point p,std::string s):
		base_plane(p,s)
	{}
};

template<class T>
struct can_collide<T,COL_OTHER>:
	public figure,
	public listable<T>
{
	can_collide(double _x,double _y,double _z):
		figure(_x,_y,_z)
	{}
	
	can_collide(double _x,double _y,double _z,std::string s):
		figure(_x,_y,_z,s)
	{}
	
	can_collide(const flomath::point& pos):
		figure(pos)
	{}

	can_collide(const flomath::point& pos,const std::string& name):
		figure(pos,name)
	{} 

};

#endif