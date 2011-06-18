#ifndef SIMCAM_HPP
#define SIMCAM_HPP

#include "engine/engine.hpp"
#include "engine/engine_interface.hpp"

class simcam: 
	public t_camera 
{
	private:
		const engine_interface* mv;
		
	public:
		simcam(const engine_interface*);
		void operator()();
};

#endif
