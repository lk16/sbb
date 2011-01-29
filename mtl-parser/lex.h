#ifndef LEX_H
#define LEX_H

int mtl_lex ();

union yystype{
	char*	str;
	double 	d;
};
#define YYSTYPE yystype

#include "PARSE_MTL.hpp"
#endif
