#include <stdio.h>

#include "parse.hpp"

t_level level_parse(std::string s){
	extern int level_parse();
	extern FILE* level_in;
	extern t_level the_level;
	
	level_in=fopen(s.c_str(),"r");
	if(!level_in){
		printf("failed to open level-file '%s' does it exist?\n",s.c_str());
		return the_level;
	}
	the_level.objects.clear();
	level_parse();
	the_level.fname=s;
	return the_level;
}


