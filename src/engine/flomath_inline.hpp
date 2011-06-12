#ifndef FLOMATH_INLINE_HPP
#define FLOMATH_INLINE_HPP

#include "flomath.hpp"
#include <GL/gl.h>

namespace flomath{

	inline point& point::normalize(){
		return *this/=length();
	}

	inline point point::normalized(){
		return *this/length();
	}

	inline std::ostream& operator<<(std::ostream& out ,const point& p){
		out << '(' << p.x << ',' << p.y << ',' << p.z << ')';
	}
	
	inline point project(const point& p,const plane& q){
		double dist = q.eval(p)/q.normal.length_sqr();
		return p-(q.normal*dist);
	}
	
	inline bool polygon::is_in_polygon(const point& p0) const{
		plane r(*this);
		point p1 = project(p0,r);
		double thoek = 0.0;
		for(unsigned i=0;i<p.size()-1;i++){
			point a = p[i]-p1;
			point b = p[i+1]-p1;
			thoek += std::acos(dotproduct(a,b)/(a.length()*b.length()));
		}
		point a = p.back()-p1,b = p.front()-p1;
		thoek += std::acos(dotproduct(a,b)/(a.length()*b.length()));
		return equals(thoek,2*pi);
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
	
	inline polygon& polygon::operator+=(const point& q){
		for(std::vector<point>::iterator i=p.begin();i!=p.end();i++){
			*i+=q;
		}
		return *this;
	}
	
	inline vector quaternion::get_vec() const{
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
		if(!equals(len,0)){
			a/=len;
			b/=len;
			c/=len;
			d/=len;
		}
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
	
	inline quaternion quaternion::operator*(const quaternion& q)const{
		quaternion res = *this;
		return res*=q;
	}
	
	inline quaternion quaternion::conj() const{
		return quaternion(a,-b,-c,-d);
	}
	
	inline quaternion quaternion::inv() const{
		return equals(length()-1,0) ? conj() : conj() * (1.0/length_sqr());
	}
	
	inline quaternion aal_rot_to_quaternion(double x,double y,double z){
		quaternion res(1,0,0,0);
		res *= quaternion(cos(x/2),flomath::point(1,0,0)*sin(x/2));
		res *= quaternion(cos(y/2),flomath::point(0,1,0)*sin(y/2));
		res *= quaternion(cos(z/2),flomath::point(0,0,1)*sin(z/2));
		return res;
	}
	
	inline quaternion aal_rot_to_quaternion(const point& p){
		return aal_rot_to_quaternion(p.x,p.y,p.z);
	}
	
	inline void glRotateq(const quaternion& q){
		glRotated(2*180/pi*acos(q.a),q.b,q.c,q.d); // BUG WTF 2*180/pi ???
	}
	
	inline quaternion quaternion::operator-() const{
		return quaternion(-a,-b,-c,-d);
	}
	
	inline double rad2deg(double x){
		return 180*x/pi;
	}
	inline double deg2rad(double x){
		return pi*x/180;
	}
}
#endif