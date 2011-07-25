#include "flomath.hpp"

namespace flomath{
	
	point::point(){
		x = y = z = 0.0;
	}

	double point::length_sqr() const
	{
		return x*x+y*y+z*z;
	}
	
	//rotates point and returns nothing
	void point::rotate(double angle,const vector& v){
		quaternion q(std::cos(angle/2),v*std::sin(angle/2));
		rotate(q);
	}
	
	/*polygon polygon::rotate(double angle,vector v){
		polygon res(*this);
		for(unsigned i=0;i<res.p.size();i++){
			res.p[i].rotate(angle,v);
		}
		return res;
	}*/
	
	void polygon::rotate(const quaternion& q){
		for(unsigned i=0;i<p.size();i++){
			p[i].rotate(q);
		}
	}
	
	
	void point::rotate(const quaternion& q){
		quaternion r(0.0,*this);
		*this = (q*r*q.inv()).get_vec();
	}
	
	plane::plane(const point& tp1,const point& tp2,const point& tp3){
		/** the equation for a plane in R3 is 
		ax+by+cz+d=0
		normal: col(a,b,c)
		*/
		vector v1(tp1),v2(tp1);
		v1 -= tp2;
		v2 -= tp3;
				
		normal = crossproduct(v1,v2);
		d = dotproduct(normal,tp1);		
	}
	
	plane::plane(const triangle& t){
		vector v1=t.p[0]-t.p[1];
		vector v2=t.p[0]-t.p[2];
		normal = crossproduct(v1,v2);
		d = dotproduct(normal,t.p[0]);
	}
	
	plane::plane(const polygon& poly)
	{
		vector v1=poly.p[0]-poly.p[1];
		vector v2=poly.p[0]-poly.p[2];
		normal = crossproduct(v1,v2);
		d = dotproduct(normal,poly.p[0]);
	}

	double plane::eval(const point& a)const{
		return dotproduct(normal,a)-d;
	}

	double distance(const point& p1,const point& p2){
		return (p1-p2).length();
	}

	/// | RC X (OFF - P) |  
	/// ------------------
	///       |RC|
	double distance(const point& p,const line3d& l){
		point tmp1 = l.offset-p;
		double res = crossproduct(l.rc,tmp1).length();
		return res/l.rc.length();
	}
	
	point project(const point& p,const line3d& l){
		point tmp1 = l.offset-p;
		point tmp2 = crossproduct(l.rc,tmp1);
		point tmp3 = crossproduct(tmp2,l.rc); 
		tmp3.normalized();
		tmp3 = tmp3 * tmp2.length()/l.rc.length();
		return p+tmp3;
	}

	vector crossproduct(const vector& a, const vector& b){
		return vector(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
	}

	double dotproduct(const vector& a,const vector& b){
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	//http://mathworld.wolfram.com/Line-LineIntersection.html
	
	//BUG this function doesn't work properly..
	bool lines_intersect(const line3d& a,const line3d& b){
		vector c,d;
		double t;
		c = crossproduct(a.rc,b.rc);
		d = crossproduct(b.offset-a.offset,b.rc);
		t = c.x/d.x;
		return (flomath::equals(c.y/d.y,t) && flomath::equals(c.z/d.z,t));
	}	
	
	point intersect_lines(const line3d& a,const line3d& b){
		/*point res;
		vector c=b.offset-a.offset;
		double s=dotproduct(
				crossproduct(c,b.rc),
				crossproduct(a.rc,b.rc)
			)/(
				crossproduct(a.rc,b.rc).length_sqr()
			);
		res=a.offset+a.rc*s;*/
		vector c,d;
		double t;
		c = crossproduct(a.rc,b.rc);
		d = crossproduct(b.offset-a.offset,b.rc);
		t = c.x/d.x;
		if(!(flomath::equals(c.y/d.y,t) && flomath::equals(c.z/d.z,t))){
			show_error("lines do not intersect at all",false);
		}
		return a.offset+ t*a.rc;
	}

	bool are_planes_paralel_in_coords(const plane& p1, const plane& p2,double vector::* axis1, double vector::* axis2){
		return (p1.normal.*axis1 * p2.normal.*axis2)  ==  (p2.normal.*axis1 * p1.normal.*axis2);
	}

	line3d get_intersect_line(const plane& p1,const plane& p2){
		/**
		*[a b]= [c]
		*[d e]= [f]
		*
		*      c*e - f*b
		* x =  ---------
		*      a*e - d*b  
		*
		*
		*-->p1x-(p1y/p2y)*p2x =p1d-(p1y/p2y)*p2d 
		*-->
		*z=0
		//*/
		
		line3d res;
		res.rc=crossproduct(p1.normal,p2.normal);
		res.rc.normalize();
		double point::* a=&point::x, point::* b=&point::y;
		
		if(!are_planes_paralel_in_coords(p1,p2,a,b)){
			res.offset.*a =    (p1.d         * p2.normal.*b - p2.d         * p1.normal.*b)
						/(p1.normal.*a * p2.normal.*b - p2.normal.*a * p1.normal.*b); 
			res.offset.*b =    (p1.d         * p2.normal.*a - p2.d         * p1.normal.*a)
						/(p1.normal.*b * p2.normal.*a - p2.normal.*b * p1.normal.*a); 
		}
		else if(b=&point::z,!are_planes_paralel_in_coords(p1,p2,a,b)){
			b =&point::z;
			res.offset.*a =    (p1.d         * p2.normal.*b - p2.d         * p1.normal.*b)
						/(p1.normal.*a * p2.normal.*b - p2.normal.*a * p1.normal.*b); 
			res.offset.*b =    (p1.d         * p2.normal.*a - p2.d         * p1.normal.*a)
						/(p1.normal.*b * p2.normal.*a - p2.normal.*b * p1.normal.*a);
		}
		else{
			show_warning("planes do not intersect, failing silently");
		}
		return res;
	}
	
	double point::length() const{
		return std::sqrt(length_sqr());
	}
	
	point::point(double nx,double ny,double nz):x(nx),y(ny),z(nz){}
	
	point point::operator+(const point& b) const{
		return point(x+b.x,y+b.y,z+b.z);
	}
	point point::operator-(const point& b)const{
		return point(x-b.x,y-b.y,z-b.z);
	}
	
	point point::operator-()const{
		return point(-x,-y,-z);
	}
	
	point& point::operator+=(const point& b){
		x+=b.x;
		y+=b.y;
		z+=b.z;
		return *this;
	}
	
	point& point::operator-=(const point& b){
		x-=b.x;
		y-=b.y;
		z-=b.z;
		return *this;
	}
	
	point point::operator*(double b)const {
		return point(x*b,y*b,z*b);
	}
	
	point point::operator/(double b) const{
		return *this * (1/b);
	}
		
	bool point::operator==(const point& b)const{
		return equals(x,b.x) && equals(y,b.y) && equals(z,b.z);
	}
	
	point operator*(double b,const point& a){
		return point(a.x*b,a.y*b,a.z*b);
	}
	
	point operator*(const point& a,double b){
		return b*a;
	}	
	
	point& point::operator*=(double b){
		x*=b;
		y*=b;
		z*=b;
		return *this;
	}
	point& point::operator/=(double b){
		x/=b;
		y/=b;
		z/=b;
		return *this;
	}
	
	void point::set(double _x,double _y,double _z){
		x=_x , y=_y , z=_z;
	}

	line3d::line3d(const point& a,const point& b):
		rc(a-b),offset(a)
	{}
	
	triangle triangle::operator+(const vector& b)const{
		return triangle(p[0]+b,p[1]+b,p[2]+b);
	}
	
	triangle::triangle(){}
	
	triangle::triangle(const point& p0, const point& p1, const point& p2){
		p[0]=p0;
		p[1]=p1;
		p[2]=p2;
	}

	polygon::polygon(const point& p1,const point& p2,const point& p3){
		p.push_back(p1);
		p.push_back(p2);
		p.push_back(p3);
	}

	polygon::polygon(
		const point& p1,
		const point& p2,
		const point& p3,
		const point& p4
	){
		p.push_back(p1);
		p.push_back(p2);
		p.push_back(p3);
		p.push_back(p4);
	}

	// TODO something would be wrong
	bool polygon::in_plane()const{
		if(p.size()<3){
			show_warning("polygon consisting of less then three points");
			return false;
		}
		if(p.size()==3){
			return true;
		}
		plane tp(p[0],p[1],p[2]);
		for(unsigned i=3;i<p.size();i++){
			if(equals(tp.eval(p[i]),0)){
				return true;
			}
		}
		return false;
	}

	void base_figure::add_triangle(const point& p0,const point& p1,const point& p2){
		polly.push_back(polygon(p0,p1,p2));
	}

	void base_figure::add_triangle(const triangle& t){
		polly.push_back(t);
	}

	double base_figure::radius()const{
		double currmax=0.0;
		for(std::vector<polygon>::const_iterator i=polly.begin();i!=polly.end();i++){
			for(std::vector<point>::const_iterator j=i->p.begin();j!=i->p.end();j++){
				double tmp = j->length();
				if(tmp>currmax){
					currmax=tmp;
				}
			}
		}
		return currmax;
	}
	
	void base_figure::add_polygon(const polygon& p){
		if(!p.in_plane()){
			show_error("polygon not in plane",false);
		}
		polly.push_back(p);
	}
	
	void base_figure::add_polygon(const point& p0,const point&p1,const point&p2,const point&p3){
		polly.push_back(polygon(p0,p1,p2,p3));
	}
	
	double distance(const point& p,const plane& x){
		return x.eval(p)/x.normal.length();
	}

}
