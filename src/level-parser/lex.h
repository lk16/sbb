#ifndef LEX_H
#define LEX_H

int level_lex ();
extern int level_lex_line;
union yystype{
	char*		str;
	double 	d;
};
#define YYSTYPE yystype

#include "PARSE_LEVEL.hpp"
//#include "y.tab.h"
#endif
