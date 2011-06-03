#include "floor.hpp"

SBB_REGISTER_ei(t_floor);

const double t_floor::height = 0.5;

void t_floor::construct_params(){
	width = fromstr<double>(get_param("w"));
	length = fromstr<double>(get_param("l"));
	update_faces();
}

void t_floor::update_faces()
{
	// half height & width
	double hl,hw; 
	hl = length/2;
	hw = width/2;

	
	
	flomath::point face_coords[5][4];
	
	// top, y=0
	face_coords[0][0] = flomath::point( hl,0, hw);
	face_coords[0][1] = flomath::point( hl,0,-hw);
	face_coords[0][2] = flomath::point(-hl,0,-hw);
	face_coords[0][3] = flomath::point(-hl,0, hw);
	
	// size x=hl
	face_coords[1][0] = flomath::point( hl,0      , hw);
	face_coords[1][1] = flomath::point( hl,0      ,-hw);
	face_coords[1][2] = flomath::point( hl,-height,-hw);
	face_coords[1][3] = flomath::point( hl,-height, hw);
	
	// size x=-hl
	face_coords[2][0] = flomath::point(-hl,0      , hw);
	face_coords[2][1] = flomath::point(-hl,0      ,-hw);
	face_coords[2][2] = flomath::point(-hl,-height,-hw);
	face_coords[2][3] = flomath::point(-hl,-height, hw);
	
	// size z=hw
	face_coords[3][0] = flomath::point(-hl,0      , hw);
	face_coords[3][1] = flomath::point( hl,0      , hw);
	face_coords[3][2] = flomath::point( hl,-height, hw);
	face_coords[3][3] = flomath::point(-hl,-height, hw);
	
	// size z=-hw
	face_coords[4][0] = flomath::point(-hl,0      ,-hw);
	face_coords[4][1] = flomath::point( hl,0      ,-hw);
	face_coords[4][2] = flomath::point( hl,-height,-hw);
	face_coords[4][3] = flomath::point(-hl,-height,-hw);
	
	flomath::polygon tmp(face_coords[0][0],face_coords[0][1],face_coords[0][2],face_coords[0][3]);
	add_polygon(tmp);
		
	drawable_face df;
	df.has_texture = true;
	df.texname = get_texture_tag(get_param("tex","floor.png")); 
	drawable_face::coord c;
	
	for(int i=0;i<4;i++){
		*(flomath::point*)&c = face_coords[0][i];
		
		c.tex = flomath::point( ((i+1)&2)?width:0, (i&2)?length:0, 0  );
		df.coords.push_back(c);
	}
	draw_faces.push_back(df);
	
	for(int i=1;i<=4;i++){
		flomath::polygon tmp(face_coords[i][0],face_coords[i][1],face_coords[i][2],face_coords[i][3]);
		add_polygon(tmp);
		
		drawable_face df;
		df.alpha = 1;
		df.r = df.g = df.r = 0.5; //default grey 
		df.has_texture = false;
		drawable_face::coord c;
		
		for(int j=0;j<=3;j++){
		*(flomath::point*)&c = face_coords[0][i];
			df.coords.push_back(c);
		}
		draw_faces.push_back(df);
	}	
}


void t_floor::set_length(double l)
{
	length = l;
	update_faces();
}

void t_floor::set_width(double w)
{
	width = w;
	update_faces();
}

