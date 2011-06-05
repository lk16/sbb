#ifndef SBB_MOVING_FLOOR_HPP
#define SBB_MOVING_FLOOR_HPP

#include <cmath>
#include <sys/time.h>

#include "floor.hpp"
#include "engine/flomath.hpp"

class moving_floor:
	public t_floor
{
public:
	
	//start and end position
	flomath::point start,end;
	
	//secs
	double period;
	
	moving_floor(t_engine& e, const flomath::point& pos):
		t_floor(e,pos)
	{
		add_interface_from_bitset(ei_stepable);
		
	}
	
	virtual void step();
	virtual void construct_params();
};

#endif