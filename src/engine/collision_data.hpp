#ifndef SBB_COLLISION_DATA_HPP
#define SBB_COLLISION_DATA_HPP

#include <algorithm>
#include <cmath>

#include "flomath.hpp"
#include "figure.hpp"
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

bool collide(const flomath::polygon&,const flomath::polygon&);

collision_data_base* collide(figure&,figure&);
collision_data_base* collide(figure&,base_sphere&);
collision_data_base* collide(base_sphere&,base_sphere&);
collision_data_base* collide(base_sphere&,figure&);

#endif