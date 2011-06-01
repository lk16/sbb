%{
#include "object-parser/lex_h.hpp"
#include <stdlib.h>
//#define DO(x) printf("%s\n",#x)
#define DO(x) return x

%}
%%
\#.* 					/*DO( COMMENT);*/
\\\n					/*DO( CONTINUATION);*/

vt[ \t]				DO( TEXTURE_VERTEX);
vn[ \t]				DO( VERTEX_NORMAL);
v[ \t]				DO( VERTEX);
fo?[ \t]				DO( FACE);
g[ \t]				DO( GROUP);
s[ \t]				DO( SMOOTHING_GROUP);
o[ \t]				DO( OBJECT);
mtllib[ \t]				DO( MTLLIB);
usemtl[ \t]				DO( USEMTL);
[0-9e+-\.]+				OBJ_PARSElval.d=atof(OBJ_PARSEtext);DO( NUM); /* een prachtige creatie niet?*/
\/					DO( SLASH);
\n                      	DO( NL);
[ \t]                 		/*DO( WS);*/
[-_0-9a-zA-Z\.]+			OBJ_PARSElval.str=strdup(OBJ_PARSEtext);DO( NAME);
%%
