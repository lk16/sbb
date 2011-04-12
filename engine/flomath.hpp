#ifndef FLOMATH_HPP
#define FLOMATH_HPP

#include <boost/array.hpp>
#include <vector>
#include <cmath>

<<<<<<< HEAD
#include "util.hpp"

namespace flomath{
	
=======
const double negligible = 0.000001;

///this does exactly what you think it does, dont execute on systems without protected memory(ancient 8086 for example)
inline void segfault(){
	*(int*)(0)=1;
}


namespace flomath{
	
	const double pi=3.1415926;
	
	template<class T>
	T acos(T x);

	template <class T>
	T abs (T p);

	template<class T,class U>
	bool equals(T x,U y);

>>>>>>> 7d795d9ad1de932c75971aa5975d8b47383a0bc9
	template <class T,class U>
	bool operator!=(T lhs,U rhs);

	struct point;
	struct quaternion;
	struct plane;
	struct polygon;
	struct triangle;
	struct line3d;
	struct base_figure;
	
	std::ostream& operator<<(std::ostream&,point);
	struct point{
		double x,y,z;
		double length() const;
		double length_sqr() const;
		point normalized();
		point& normalize();
		
		friend std::ostream& operator<<(std::ostream&,point);
		
<<<<<<< HEAD
		point();
=======
		point(){}
>>>>>>> 7d795d9ad1de932c75971aa5975d8b47383a0bc9
		point(double,double,double);
		point operator+(const point& b)const;
		point operator-(const point& b)const;
		point operator-()const;
		point& operator+=(const point& b);
		point& operator-=(const point& b);
		point& operator*=(double b);
		point& operator/=(double b);
		point operator*(double b)const;
		point operator/(double b) const;
		bool operator==(const point& b)const;
		void set(double,double,double);
		void rotate(quaternion);
		void rotate(double,point);
	};
	
	typedef point vector;
	vector crossproduct(const vector&,const vector&);
	double dotproduct(const vector&,const vector&);
	void   normalize(vector&);

	double distance(const point&,const point&);
	double distance(plane,point);
	double distance(const flomath::point& p,const flomath::line3d& l);
	
	///DONT USE THIS!
	point turn(const point&,const vector&);

	struct triangle{
		flomath::point p[3];
		triangle operator+(const vector& b) const;
		triangle();
		triangle(const flomath::point& p0, const flomath::point& p1, const flomath::point& p2);
	};
	
	triangle operator*(double lhs,triangle rhs);
	
	struct plane{
		flomath::vector normal;
		double d;
		
		plane(const point&,const point&,const point&);
		plane(const triangle&);
		plane(const polygon&);
		plane(){}
		double eval(const point&)const;
	};
	

	/*
	 *RC*t+offset=X,Y,Z
	 *
	 */
	struct line3d{
		line3d(){}
		line3d(const point&,const point&);
		vector rc;
		vector offset;
	};

	line3d get_intersect_line(const plane&,const plane&);
	bool lines_intersect(const line3d&,const line3d&);
	point intersect_lines(const line3d&,const line3d&);
	
	
	struct base_figure{
		std::vector<polygon> polly;//wants a cracker
	
		void add_triangle(const flomath::point&,const flomath::point&,const flomath::point&);
		void add_triangle(const triangle&);	
		void add_polygon(const polygon& p);
		void add_polygon(const flomath::point&,const flomath::point&,const flomath::point&,const flomath::point&);
		double radius()const;
	};
	
	struct quaternion{
		double a,b,c,d;
		quaternion(){}
		quaternion(double _a,double _b,double _c,double _d): a(_a),b(_b),c(_c),d(_d){}
		quaternion(double a,vector v):a(a),b(v.x),c(v.y),d(v.z){}
		quaternion(double _a):a(_a){}
		
		vector get_vec();
		quaternion& operator+=(const quaternion& q);
		quaternion operator+(const quaternion& q)const;
		quaternion operator-();
		void normalize();
		quaternion normalized()const;
		double length_sqr()const;
		double length()const;
		quaternion& operator*=(const quaternion& q);
		quaternion operator *(const quaternion& q)const;
		quaternion conj();
		quaternion inv();
		
	};

	void glRotateq(quaternion);

	struct polygon{
		std::vector<flomath::point> p;
		
		polygon(){}
		polygon(const point&,const point&,const point&);
		polygon(const point&,const point&,const point&,const point&); 
 		polygon(const triangle&);

		polygon& operator+=(point q);
		bool in_plane()const;	
		//polygon rotate(double,vector);
		void rotate(quaternion q);
	};
	
	point project(const point&,const plane& );
	point project(const point&,const line3d&);
	
	bool is_in_polygon(const point& p0,const polygon& q);
}

#include "flomath_inline.hpp"

#endif
