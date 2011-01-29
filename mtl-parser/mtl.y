%{

#include <stdio.h>
#include <map>
#include <string>
#include "mtl-parser/lex.h"
#include "mtl-parser/parse.hpp"

std::map<std::string,material>* materials;
material* mtl_curr=0;
int mtl_regel=1;

extern "C" void yyerror(const char *str){
	    fprintf(stderr,"YACC:error: %s\n",str);
	    fprintf(stderr,"YACC:error: on line %i\n",mtl_regel);
}
 
extern "C" int mtl_wrap(){
	mtl_regel=1;
	return 1;
} 

%}
%token NL NUM NAME NS NEW_MTL ALPHA ILLUM KD KA KS KE MAPKD

%%
start	: file					
	;

file	: file NL line			{mtl_regel++;}
		| line			{mtl_regel++;}
		;

line	:   		NEW_MTL		NAME			{mtl_curr=&(*materials)[$2.str];}	
		|	ALPHA			NUM			{mtl_curr->alpha=$2.d;}
		|	ILLUM			NUM			{mtl_curr->illum=$2.d;}
		|	KD			NUM NUM NUM		{mtl_curr->kd=colour($2.d,$3.d,$4.d);}
		|	KA			NUM NUM NUM		{mtl_curr->ka=colour($2.d,$3.d,$4.d);}
		|	KS			NUM NUM NUM		{mtl_curr->ks=colour($2.d,$3.d,$4.d);}
		|	KE			NUM NUM NUM 	{mtl_curr->ke=colour($2.d,$3.d,$4.d);}
		|	MAPKD			NAME			{mtl_curr->filename=$2.str;}
		|	NS			NUM			{mtl_curr->shinyness=$2.d;}
		|   /*  empty */
		;













		
