#ifndef SBB_COLLISION_TEST_HPP
#define SBB_COLLISION_TEST_HPP

#include <algorithm>
#include <cmath>
#include <vector>

#include "flomath.hpp"

struct collision_data_base;

template<class T>
struct collision_data;

template <class T, class U>
struct collision_impl
{
	static void apply(){
		for(typename std::vector<T*>::iterator i=T::list.begin();i!=T::list.end();i++){
			for(typename std::vector<U*>::iterator j=U::list.begin();j!=U::list.end();j++){
				if(collision_data_base* data= collide(**i,**j)){
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
