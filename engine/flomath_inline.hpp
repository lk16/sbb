#ifndef FLOMATH_INLINE_HPP
#define FLOMATH_INLINE_HPP

#include "flomath.hpp"
#include <GL/gl.h>

namespace flomath{

	template <class T>
	T abs (T p){
		return (p<0)?-p:p;
	}
	
	template<class T>
	T acos(T x){
		T ax=flomath::abs(x);
		return std::acos(ax>1?x/ax:x);
	}
	
	template<class T,class U>
	bool equals(T x,U y){
		return flomath::abs(x-y)<negligible;
	}
	
	template <class T,class U>
	bool operator!=(T lhs,U rhs){
		return !(lhs==rhs);
	}
	
	inline point& point::normalize(){
		return *this/=length();
	}

	inline point point::normalized(){
		return *this/length();
	}

	inline std::ostream& operator<<(std::ostream& out ,point p){
		out <<'('<<p.x<<','<<p.y<<','<<p.z<<')';
	}
	inline point project(const point& p,const plane& q){
		double dist = q.eval(p)/q.normal.length_sqr();
		return p-(q.normal*dist);
	}
	
	inline bool is_in_polygon(const point& p0,const polygon& q){
		plane r(q);
		point p1 = project(p0,r);
		double thoek = 0.0;
		for(unsigned i=0;i<q.p.size()-1;i++){
			point a = q.p[i]-p1;
			point b = q.p[i+1]-p1;
			thoek += std::acos(dotproduct(a,b)/(a.length()*b.length()));
		}
		point a = q.p.back()-p1,b = q.p.front()-p1;
		thoek += std::acos(dotproduct(a,b)/(a.length()*b.length()));
		return abs(thoek - 2*pi) < 0.1 ;
	}
	
	
	inline triangle operator*(double lhs,triangle rhs){
		triangle res(rhs);
		for(int i=0;i<3;i++){
			res.p[i]*=lhs;
		}
		return res;
	}

	inline polygon::polygon(const triangle& t){
		p.push_back(t.p[0]);
		p.push_back(t.p[1]);
		p.push_back(t.p[2]);
	}
	
	inline polygon& polygon::operator+=(point q){
		for(std::vector<point>::iterator i=p.begin();i!=p.end();i++){
			*i+=q;
		}
		return *this;
	}
	
	inline vector quaternion::get_vec(){
		return vector(b,c,d);
	}
	
	inline quaternion& quaternion::operator+=(const quaternion& q){
		a += q.a;
		b += q.b;
		c += q.c;
		d += q.d;
		return *this;
	}
	inline quaternion quaternion::operator+(const quaternion& q)const{
		quaternion res = *this;
		return res+=q;
	}
	inline void quaternion::normalize(){
		double len=length();
		a/=len;
		b/=len;
		c/=len;
		d/=len;
	}
	inline quaternion quaternion::normalized()const{
		quaternion res(*this);
		res.normalize();
		return res;
	}
	inline double quaternion::length_sqr()const{
		return a*a+b*b+c*c+d*d;
	}
	inline double quaternion::length()const{
		return std::sqrt(length_sqr());
	}
	inline quaternion& quaternion::operator*=(const quaternion& q){
		quaternion copy(*this);
		copy.a = a*q.a - b*q.b - c*q.c - d*q.d;
		copy.b = a*q.b + b*q.a + c*q.d - d*q.c;
		copy.c = a*q.c - b*q.d + c*q.a + d*q.b;
		copy.d = a*q.d + b*q.c - c*q.b + d*q.a;
		return *this = copy;
	}
	inline quaternion quaternion::operator *(const quaternion& q)const{
		quaternion res = *this;
		return res*=q;
	}
	inline quaternion quaternion::conj(){
		return quaternion(a,-b,-c,-d);
	}
	inline quaternion quaternion::inv(){
		if(abs(length()-1)<negligible){
			return conj();
		}
		else{
			return conj()* (1/length_sqr()); 
		}
	}
	
	inline quaternion aal_rot_to_quaternion(double x,double y,double z){
		quaternion res(1,0,0,0);
		res *= quaternion(cos(x/2),flomath::point(1,0,0)*sin(x/2));
		res *= quaternion(cos(y/2),flomath::point(0,1,0)*sin(y/2));
		res *= quaternion(cos(z/2),flomath::point(0,0,1)*sin(z/2));
		return res;
	}
	
	inline quaternion aal_rot_to_quaternion(point p){
		return aal_rot_to_quaternion(p.x,p.y,p.z);
	}
	
	inline void glRotateq(quaternion q){
		glRotated(2*180/pi*acos(q.a),q.b,q.c,q.d);
	}
	
	inline quaternion quaternion::operator-(){
		return quaternion(-a,-b,-c,-d);
	}
}
#endif