%{
#include "level-parser/lex.h"
#include <stdlib.h>
//#define DO(x) printf("%s\n",#x)
#define DO(x) return x
int level_lex_line=0;
%}
%%
\#.* 					level_lex_line++;DO( NL);
\{					DO( LB);
\}					DO( RB);

\"(\\.|[^\"])*\" 			{level_lval.str=strdup(level_text+1);level_lval.str[strlen(level_lval.str)-1]='\0';DO( QSTRING );}

[0-9e+-\.]+				level_lval.d=atof(level_text);DO( NUM);
\n+   				level_lex_line++;DO( NL);
[ \t]                 			/*DO( WS);*/
LEVEL[ \t]				DO( LEVEL);
CAMERA[ \t]				DO( CAMERA);
[-_0-9a-zA-Z\.]+			level_lval.str=strdup(level_text);DO( NAME);
%%
