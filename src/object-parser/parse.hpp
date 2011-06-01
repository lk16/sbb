#ifndef PARSE_HPP
#define PARSE_HPP
#include <vector>
#include <boost/filesystem.hpp>
#include "engine/flomath.hpp"
#include "mtl-parser/parse.hpp"


int OBJ_PARSElex ();
int OBJ_PARSEparse();
typedef flomath::point tex_coords;
typedef flomath::point vnormal;
typedef flomath::point t_vertex;
struct face_triple{
	int vert_ind;
	int tex_ind;
	int norm_ind;
};
struct t_face{
	t_face(material* m):mtl(m){}
	std::vector<face_triple> data;
	material* mtl;
};
typedef std::string t_group;
typedef int s_group;
extern boost::filesystem::path obj_folder;

struct obj_file{
	std::vector<tex_coords> texture_coordinates;
	std::vector<vnormal> normals;
	std::vector<t_vertex> vertices;
	std::vector<t_face> faces;
};
typedef std::map<std::string,obj_file> obj_file_map;

obj_file obj_parse(const std::string&);

#endif
