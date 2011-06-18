#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "engine/collision_test.hpp"
#include "engine/engine.hpp"
#include "engine/util.hpp"
#include "engine/can_collide.hpp"
#include "register/register.hpp"


//all floors moeten hier veilig van kunnen inheriten.
class t_floor:
	public can_collide<t_floor,COL_OTHER>
{
	double width,length;

public:
	static const double height;
	
	t_floor(const flomath::point& pos):
		can_collide<t_floor,COL_OTHER>(pos)
	{}
	
	void update_faces();
	void set_width(double w);
	void set_length(double l);
	
	virtual void construct_params();
	
	double get_width(){
		return width;
	}
	
	double get_length(){
		return length;
	}
	
	
};

#endif
