#ifndef SBB_UTIL_HPP
#define SBB_UTIL_HPP

enum doubly_owned_bool{ 
	DOB_FALSE=0,
	DOB_TRUE=1,
	DOB_TRY_FALSE,
	DOB_TRY_TRUE 
};

namespace flomath{

	const double negligible = 0.000001;
	const double pi = 3.1415926;

	template <class T>
	inline T abs(T p){
		return (p<0)?-p:p;
	}

	template<class T>
	inline T acos(T x){
		T ax=flomath::abs(x);
		return std::acos(ax>1 ? x/ax : x);
	}

	template<class T,class U>
	inline bool equals(T t,U u){
		return flomath::abs(t-u)<negligible;
	}

}

//this causes a segmentation fault, use with care!!!
inline void segfault(){
	*(int*)(0)=1;
}

template <class T,class U>
inline bool operator!=(T lhs,U rhs){
	return !(lhs==rhs);
}

template<class T>
std::string tostr(T x){
	std::stringstream buff;
	buff << x;
	return buff.str();
}

template<class T>
T fromstr(std::string x){
	std::stringstream buff(x);
	T temp;
	buff >> temp;
	return temp;
}

#endif