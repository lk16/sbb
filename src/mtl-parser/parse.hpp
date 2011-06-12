#include <map>
#include <string>
#include <string>
#include <stdio.h>
#include <iostream>
#include <boost/filesystem.hpp>

struct colour{
	colour():r(0),g(0),b(0){}
	colour(double nr,double ng,double nb):r(nr),g(ng),b(nb){}
	double r,g,b;
};

struct material{
	material():alpha(1),shinyness(0),illum(1),filename(0){}
	double alpha,shinyness;
	char illum;
	colour kd,ka,ks,ke;
	char*  filename;
};

typedef std::map<std::string,material> t_materials;
typedef std::map<std::string,std::map<std::string,material> > t_mtl_map;

extern t_materials mtl_parse(const std::string& s);


