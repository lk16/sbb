#include "parse.hpp"

#include <stdio.h>
#include <iostream>
#include <map>

#include <gtkmm/messagedialog.h>

extern std::vector<tex_coords> *texture_coordinates;
extern std::vector<vnormal> *normals;
extern std::vector<t_vertex> *vertices;
extern std::vector<t_face> *faces;

extern FILE* OBJ_PARSEin;
boost::filesystem::path obj_folder;


obj_file_map file_map;

obj_file obj_parse(const std::string& file)
{
	if(file_map.find(file)!=file_map.end()){
		return file_map[file];
	}

	boost::filesystem::path p ( file );
	obj_folder=p.parent_path();
	FILE* f=fopen(file.c_str(),"r");
	if(!f){
		std::cerr << ("file " + file + " not found\n");
		Gtk::MessageDialog m("file " + file + " not found");
		m.run();
		return obj_file();
	}
	
	std::cout << "parsing file : "<< file << std::endl;
	
	OBJ_PARSEin=f;
	
	obj_file& this_file=file_map[file];
	texture_coordinates=&this_file.texture_coordinates;
	normals=&this_file.normals;
	faces=&this_file.faces;
	vertices=&this_file.vertices;
	

	OBJ_PARSEparse();
	return this_file;
}
