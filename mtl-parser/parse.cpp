#include <string>
#include <stdio.h>
#include <iostream>

#include "parse.hpp"
#include <boost/filesystem.hpp>

extern boost::filesystem::path obj_folder;
t_mtl_map mtl_map;


t_materials mtl_parse(std::string s){
	extern int mtl_parse();
	extern FILE* mtl_in;
	extern std::map<std::string,material>* materials;
	

	std::string fname = (obj_folder/s).file_string();
	if(mtl_map.find(fname)==mtl_map.end()){
		mtl_in=fopen(fname.c_str(),"r");
		if(!mtl_in){
			printf("failed to open mtl-file '%s' does it exist?\n",s.c_str());
			return t_materials();
		}
		std::cout << "parsing file : " << (obj_folder/s) << "\n";
		extern int mtl_regel;
		mtl_regel = 1;
		materials = &mtl_map[fname];
		mtl_parse();
	}
	return mtl_map[fname];
}

