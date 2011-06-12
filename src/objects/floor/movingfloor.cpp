#include "movingfloor.hpp"

SBB_REGISTER_ei(moving_floor);

void moving_floor::step()
{
	timeval tv;
	gettimeofday(&tv,NULL);
	double time = tv.tv_sec + tv.tv_usec/1000000.0;
	double time_mod_period = time - (time/(std::floor(time/period)));
	flomath::vector diff = end-start;
	*(flomath::point*)this = start + (0.5-0.5*std::cos(2*(time_mod_period/period + flomath::pi)))*diff;
}

void moving_floor::construct_params()
{
	t_floor::construct_params();
	start.x = fromstr<double>(get_param("startx"));
	start.y = fromstr<double>(get_param("starty"));
	start.z = fromstr<double>(get_param("startz"));
	end.x = fromstr<double>(get_param("endx"));
	end.y = fromstr<double>(get_param("endy"));
	end.z = fromstr<double>(get_param("endz"));
	period = fromstr<double>(get_param("period"));
}
