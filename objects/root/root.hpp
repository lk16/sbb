#include "engine/collision_test.hpp"

struct engine;
struct carrot : can_collide<carrot,COL_OTHER>
{
	carrot(engine*,double,double,double);
};