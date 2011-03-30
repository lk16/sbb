#include <cassert>
#include "floor.hpp"

#include "register/register.hpp"

SBB_REGISTER_ei(tiled_floor);


tiled_floor::tiled_floor(t_engine& e,double _x,double _y,double _z):
	t_floor(e,flomath::point(_x,_y,_z),"data/tiled_floor.obj"
	)
{
	*static_cast<engine_interface*>(this)=engine_interface(e ,ei_has_texture | ei_drawable);
	type="tiled_floor";
}


