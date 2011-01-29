%{
#include "mtl-parser/lex.h"
#include <stdlib.h>
//#define DO(x) printf("%s\n",#x)
#define DO(x) return x

%}
%%
\#.* 					/*DO( COMMENT);*/
\\\n					/*DO( CONTINUATION);*/

Ns[ \t]					DO(NS);
newmtl[ \t]				DO(NEW_MTL);
d[ \t]					DO(ALPHA);
illum[ \t]				DO(ILLUM);	
Kd[ \t]					DO(KD);
Ka[ \t]					DO(KA);
Ks[ \t]					DO(KS);
Ke[ \t]					DO(KE);
map_Kd[ \t]				DO(MAPKD);
[0-9e+-\.]+				mtl_lval.d=atof(mtl_text);DO( NUM);
\n                      DO( NL);
[ \t]                 	/*DO( WS);*/
[-_0-9a-zA-Z\.]+		mtl_lval.str=strdup(mtl_text);DO( NAME);
%%
