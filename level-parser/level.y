%{

#include <stdio.h>
#include <string>

#include "level-parser/lex.h"
#include "level-parser/parse.hpp"


extern "C" void yyerror(const char *str){
	    fprintf(stderr,"YACC:error: %s on line %i\n",str,level_lex_line);
}
 
extern "C" int level_wrap(){
	return 1;
} 

t_level the_level;

%}
%token NL NUM NAME LEVEL CAMERA QSTRING LB RB

%%
start	: file					
	;

file	: level  
	;
	
nl	: nl NL
	| NL
	;

nl_opt: nl
	|
	;
	
level	: nl_opt LEVEL QSTRING LB nl objects nl RB nl_opt 	{the_level.name=$3.str;}
	;

objects	: objects nl object
		| object
		;

object: object_impl
	| camera
	;


object_impl	: NAME NUM NUM NUM  NUM NUM NUM QSTRING	{the_level.objects.push_back(new object($1.str,$2.d,$3.d,$4.d,$5.d,$6.d,$7.d,$8.str));}
		| NAME NUM NUM NUM  NUM NUM NUM 		{the_level.objects.push_back(new object($1.str,$2.d,$3.d,$4.d,$5.d,$6.d,$7.d));}
		| NAME NUM NUM NUM 				{the_level.objects.push_back(new object($1.str,$2.d,$3.d,$4.d));}
		| NAME NUM NUM NUM QSTRING		{the_level.objects.push_back(new object($1.str,$2.d,$3.d,$4.d,$5.str));}
		| NAME						{the_level.objects.push_back(new object($1.str));}
		| NAME QSTRING				{the_level.objects.push_back(new object($1.str,$2.str));}
		; 

camera	:CAMERA NAME LB nl_opt object_impl nl_opt RB		
									{
										the_level.camera.name=$2.str;
										the_level.camera.companion_index=the_level.objects.size()-1;
									}
		;
