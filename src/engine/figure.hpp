#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>

#include <GL/gl.h>
#include <GL/glu.h>
#include <boost/array.hpp>

#include "flomath.hpp"
#include "object-parser/parse.hpp"
#include "engine_interface.hpp"

struct drawable_face{
	bool has_texture;
	
	union{
		GLuint texname;
		struct{
			double alpha,r,g,b;
		};
	};
	
	struct coord: flomath::point{
		flomath::point tex;
	};
	std::vector<coord> coords;
	
	void draw();
};


class figure:
	public engine_interface,
	public flomath::base_figure
{
	std::string file;
	void add_face(const t_face& arg1,const obj_file& arg2);

protected:
	std::vector<drawable_face> draw_faces;	
	
public:
	const char* type;
	figure(const flomath::point&,const flomath::base_figure&);
	figure(const flomath::point&,const std::string&);
	figure(const flomath::point&);
	figure(double,double,double,const std::string&);
	figure(double,double,double);
	virtual void draw();
	virtual void load_textures();
	static unsigned get_texture_tag(const std::string& fname);
	
	void draw_col_fig();
	void file2fig(const std::string&);



};


#endif
