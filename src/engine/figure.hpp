#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <vector>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include <GL/gl.h>
#include <GL/glu.h>
#include <boost/array.hpp>

#include "flomath.hpp"
#include "engine_interface.hpp"
#include "engine.hpp"
#include "object-parser/parse.hpp"

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

struct figure:
	public virtual flomath::point,
	public engine_interface,
	public flomath::base_figure
{
	const char* type;
	figure(t_engine&,const flomath::point&,flomath::base_figure);
	figure(t_engine&,const flomath::point&,const std::string&);
	figure(t_engine&,const flomath::point&);
	figure(t_engine&,double,double,double,const std::string&);
	figure(t_engine&,double,double,double);
	virtual void draw();
	virtual void load_textures();
	static unsigned get_texture_tag(const std::string& fname);
	
	void draw_col_fig();
	void file2fig(const std::string&);

protected:
	std::vector<drawable_face> draw_faces;
	
private:
	std::string file;
	void add_face(const t_face& arg1,const obj_file& arg2);
};

#endif
