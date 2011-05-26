#ifndef FLOOR_HPP
#define FLOOR_HPP

#include "engine/collision_test.hpp"
#include "engine/engine.hpp"
#include "engine/util.hpp"


//alle floors moeten hier veilig van kunnen inheriten.
class t_floor:
	public can_collide<t_floor,COL_OTHER>
{
	double width,length;
	static const double height = 0.5;

public:
	t_floor(t_engine& e,const flomath::point& pos,const std::string& name):
		can_collide<t_floor,COL_OTHER>(e,pos,name)
	{}
	
	virtual void construct_param(){
		width = fromstr<double>(get_param('w'));
		length = fromstr<double>(get_param('l'));
		
		// half height & width
		double hl,hw; 
		hl = length/2;
		hw = width/2;
		
		flomath::point face_coords[5][4] = {
			{{hl,0,hw},{hl,0,-hw},{-hl,0,hw},{-hl,0,-hw}}, 
			
			{{hl,0,hw},{hl,0,-hw},{hl,-height,hw},{hl,-height,-hw}},
			{{-hl,0,hw},{-hl,0,-hw},{-hl,-height,hw},{-hl,-height,-hw}},
			
			{{-hl,0,hw},{hl,0,hw},{hl,-height,hw},{-hl,-height,hw}},
			{{-hl,0,-hw},{hl,0,-hw},{hl,-height,-hw},{-hl,-height,-hw}},
		};
		
		flomath::polygon tmp(face_coords[0][0],face_coords[0][1],face_coords[0][2],face_coords[0][3]);
		add_polygon(tmp);
		
		
		
		drawable_face df;
		df.has_texture = true;
		df.texname = get_texture_tag(get_param("tex","vloer.png"));
		drawable_face::coord c;
		
		*(flomath::point*)&c = face_coords[0][0];
		c.tex = flomath::point(0,0,0);
		df.coords.push_back(c);
	
		*(flomath::point*)&c = face_coords[0][1];
		c.tex = flomath::point(1,0,0);
		df.coords.push_back(c);
		
		*(flomath::point*)&c = face_coords[0][2];
		c.tex = flomath::point(1,0,1);
		df.coords.push_back(c);
	
		*(flomath::point*)&c = face_coords[0][3];
		c.tex = flomath::point(0,0,1);
		df.coords.push_back(c);
		drawable_faces.push_back(df);
		
		for(int i=1;i<=4;i++){
			flomath::polygon tmp(face_coords[i][0],face_coords[i][1],face_coords[i][2],face_coords[i][3]);
			add_polygon(tmp);
			
			drawable_face df;
			df.alpha = 1;
			df.r = df.g = df.r = 0.5; //default grey 
			df.has_texture = false;
			drawable_face::coord c;
			
			for(int j=0;j<=3;j++){
				*(flomath::point*)&c = face_coords[i][j];
				df.coords.push_back(c);
			}
			drawable_faces.push_back(df);
		}
	}
	
	double get_width(){
		return width;
	}
	
	double get_height(){
		return length;
	}
	
	void set_width(double w);
	void set_height(double h);
	
	
};

#endif
