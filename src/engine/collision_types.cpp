#include "collision_types.hpp"

#include "figure.hpp"
#include "flomath.hpp"

base_sphere::base_sphere(const flomath::point& pos,double _r,bool gentri=true):
	figure(pos),
	r(_r)
{
	if(gentri) generate_triangle(20);
}


void base_sphere::generate_triangle(unsigned precision){
	std::vector<std::vector< flomath::point > > data;
	const double hoek=flomath::pi/precision;
	for(double u=2*flomath::pi;u>0;u-=hoek){
		double y=r*std::cos(u);
		double rcircle=r*std::sin(u);
		std::vector< flomath::point > curr;
		for(double v=2*flomath::pi;v>0;v-=hoek){
			double x=rcircle*std::cos(v);
			double z=rcircle*std::sin(v);
			curr.push_back(flomath::point(x,y,z));
		}
		data.push_back(curr);
	}
	for(size_t i=0;i<data.size()-1;i++){
		for(size_t j=0;j<data[i].size()-1;j++){
			add_polygon(
				data[i][j],
				data[i][j+1],
				data[i+1][j+1],
				data[i+1][j]
			);
		}
		add_polygon(
			data[i][0],
			data[i].back(),
			data[i+1].back(),
			data[i+1][0]
		);
	}
}

base_plane::base_plane(const flomath::point& _pos, const flomath::polygon& p):
	figure(_pos)
{
	if(p.p.size()>2){
		*static_cast<flomath::plane*>(this) = flomath::plane(
			_pos+p.p[0],
			_pos+p.p[1],
			_pos+p.p[2]
		);
	}
	add_polygon(p);
}

base_plane::base_plane(const flomath::point& _pos, const std::string& s):
	figure(_pos,s)
{}

void base_plane::load_textures(){
	figure::load_textures();
	*static_cast<flomath::plane*>(this) = flomath::plane(
		pos+polly[0].p[0],
		pos+polly[0].p[1],
		pos+polly[0].p[2]
	);
}

double base_sphere::radius() const
{
	return r;
}
