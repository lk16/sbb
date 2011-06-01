#include "root.hpp"
#include "register/register.hpp"
#include "engine/engine.hpp"

SBB_REGISTER_ei(carrot);

carrot::carrot(engine* e, double x, double y, double z): can_collide<carrot,COL_OTHER>(e,x,y,z,"carrot.obj")

{

}
