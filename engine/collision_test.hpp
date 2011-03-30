#ifndef COLLISION_TEST_HPP
#define COLLISION_TEST_HPP

#include <algorithm>
#include <cmath>

#include "figure.hpp"
#include "flomath.hpp"
#include "collision_types.hpp"

struct collision_data_base{
	flomath::polygon* p_mine;
	flomath::polygon* p_other;
	collision_data_base(flomath::polygon* pm,flomath::polygon* po):p_mine(pm),p_other(po){}
	void swap();
};

template<class T>
struct collision_data:
	collision_data_base
{
	T* other;
	
	collision_data(collision_data_base* base, T* _o):collision_data_base(*base),other(_o){}
	
	operator T&(){
		return *other;
	}
};




extern bool debug;

bool collide(flomath::polygon,flomath::polygon);

collision_data_base* collide(figure&,figure&);
collision_data_base* collide(figure&,base_sphere&);
collision_data_base* collide(base_sphere&,base_sphere&);
collision_data_base* collide(base_sphere&,figure&);


///T::list will be published and accounted as a std::vector< T* > of all the instances of T 

template <class T>
struct listable{
	static std::vector<T*> list;
	listable(){ list.push_back(static_cast<T*>(this)); }
	~listable(){ list.erase(std::remove(list.begin(),list.end(),static_cast<T*>(this)),list.end()); }
};

template <class T>
std::vector<T*> listable<T>::list;


enum {COL_SPHERE,COL_PLANE,COL_OTHER};

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
	can_collide(t_engine& e, double _x,double _y,double _z,double r,bool gentri=true):
		base_sphere(e,flomath::point(_x,_y,_z),r,gentri)
	{}
	
	can_collide(t_engine& e,const flomath::point& pos,double r,bool gentri=true):
		base_sphere(e,pos,r,gentri)
	{}
};

template<class T>
struct can_collide<T,COL_PLANE>:
	public base_plane,
	public listable<T>
{
	can_collide(t_engine& e, double _x,double _y,double _z,const flomath::polygon& pol):
		base_plane(e,flomath::point(_x,_y,_z),pol)
	{}
	
	can_collide(t_engine& e,const flomath::point& pos,const flomath::polygon& pol):
		base_plane(e,pos,pol)
	{}
	
	can_collide(t_engine& e, double _x,double _y,double _z,const std::string& s):
		base_plane(e,flomath::point(_x,_y,_z),s)
	{}
};

template<class T>
struct can_collide<T,COL_OTHER>:
	public figure,
	public listable<T>
{
	can_collide(t_engine& e, double _x,double _y,double _z):
		figure(e,_x,_y,_z)
	{}
	
	can_collide(t_engine& e, double _x,double _y,double _z,std::string s):
		figure(e,_x,_y,_z,s)
	{}
	
    can_collide(t_engine& e,const flomath::point& pos):
		figure(e,pos)
	{}

	can_collide(t_engine& e,const flomath::point& pos,const std::string& name):
		figure(e,pos,name)
	{} 

};


template <class T, class U>
struct collision_impl
{
	static void apply(){
		for(typename std::vector<T*>::iterator i=T::list.begin();i!=T::list.end();i++){
			for(typename std::vector<U*>::iterator j=U::list.begin();j!=U::list.end();j++){
				if(collision_data_base *data= collide(**i,**j)){
					collision_data<U> a(data,*j);
					(*i)->collide(a);
				}
			}
		}
		
	}
};

template <class T>
struct collision_impl<T,T>
{
	static void apply(){
		for(typename std::vector<T*>::iterator i=T::list.begin();i!=T::list.end();i++){
			for(typename std::vector<T*>::iterator j=T::list.begin();j!=T::list.end();j++){
				if(*i==*j){
					continue;
				}
				if(collide(**i,**j)){
					(*i)->collide(**j);
				}
			}
		}
	}
};


/**T must provide a memberfunction collide which can accept U e.g T::collide(U)
and it should probably inherit from listable<T>,though anything with a similar interface is allowed 
special care is taken when T == U ; self collision won't be reported.
*/
template <class T, class U>
void collision(){
	collision_impl<T,U>::apply();
}

#endif
