#ifndef OBJECT_LEX_HPP
#define OBJECT_LEX_HPP

struct face_triple;
struct t_face;
union yystype{
	face_triple* ft;
	t_face*      face;
	double 	 d;
	char*		 str;
};

#define YYSTYPE yystype

#include "PARSE_OBJ.hpp"
#endif