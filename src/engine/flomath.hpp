#ifndef FLOMATH_HPP
#define FLOMATH_HPP

#include <boost/array.hpp>
#include <vector>
#include <cmath>

#include "util.hpp"

namespace flomath{
	
	struct point;
	struct quaternion;
	struct plane;
	struct polygon;
	struct triangle;
	struct line3d;
	struct base_figure;
	
	typedef point vector;
	
	struct point{
		double x,y,z;
		
		point();
		point(double,double,double);
		
		friend std::ostream& operator<<(std::ostream&,const point&);
		
		double length() const;
		double length_sqr() const;
		point normalized();
		point& normalize();
		
		

		point operator+(const point&)const;
		point operator-(const point&)const;
		point& operator+=(const point&);
		point& operator-=(const point&);
		point& operator*=(double);
		point& operator/=(double);
		point operator*(double) const;
		point operator/(double) const;
		point operator-()const;

		bool operator==(const point&)const;
		void set(double,double,double);
		void rotate(const quaternion&);
		void rotate(double,const flomath::vector&);
	};
	
	point operator*(double,const point&);
	
	
	
	vector crossproduct(const vector&,const vector&);
	double dotproduct(const vector&,const vector&);
	double distance(const point&,const point&);
	double distance(const point&,const plane&);
	double distance(const point&,const line3d&);
	point project(const point&,const plane&);
	point project(const point&,const line3d&);
	
	struct triangle{
		point p[3];
		
		triangle();
		triangle(const point&,const point&,const point&);
		
		triangle operator+(const vector&)const;
	};
	
	triangle operator*(double,triangle);
	
	
	struct plane{
		vector normal;
		double d;
		
		plane(const point&,const point&,const point&);
		plane(const triangle&);
		plane(const polygon&);
		plane(){}
		double eval(const point&)const;
	};
	

	//RC*t+offset=X,Y,Z
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
		std::vector<polygon> polly;
	
		void add_triangle(const point&,const point&,const point&);
		void add_triangle(const triangle&);	
		void add_polygon(const polygon&);
		void add_polygon(const point&,const point&,const point&,const point&);
		double radius() const;
	};
	
	
	struct quaternion{
		double a,b,c,d;

		quaternion(){}
		quaternion(double _a,double _b,double _c,double _d): a(_a),b(_b),c(_c),d(_d){}
		quaternion(double a,const vector& v):a(a),b(v.x),c(v.y),d(v.z){}
		quaternion(double _a):a(_a){}	
		vector get_vec() const;
		quaternion& operator+=(const quaternion&);
		quaternion operator+(const quaternion&)const;
		quaternion operator-() const;
		void normalize();
		quaternion normalized() const;
		double length_sqr() const;
		double length() const;
		quaternion& operator*=(const quaternion&);
		quaternion operator*(const quaternion&) const;
		quaternion conj() const;
		quaternion inv() const;
	};
	
	void glRotateq(const quaternion&);

	
	struct polygon{
		std::vector<point> p;
		
		polygon(){}
		polygon(const point&,const point&,const point&);
		polygon(const point&,const point&,const point&,const point&); 
 		polygon(const triangle&);
		polygon& operator+=(const point&);
		bool in_plane()const;	
		void rotate(const quaternion&);
		bool is_in_polygon(const point&)const;
		//polygon rotate(double,vector);
	};

	double deg2rad(double);
	double rad2deg(double);
}

#include "flomath_inline.hpp"

#endif
