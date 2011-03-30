%{

#include <stdio.h>
#include <vector>
#include <algorithm>
#include "engine/flomath.hpp"
#include "mtl-parser/parse.hpp"

int OBJ_PARSElex ();
typedef flomath::point tex_coords;
typedef flomath::point vnormal;
typedef flomath::point t_vertex;
struct face_triple{
	face_triple(int a,int b=-1,int c=-1):vert_ind(a),tex_ind(b),norm_ind(c){}
	int vert_ind;
	int tex_ind;
	int norm_ind;
};
struct t_face{
	t_face(material* m):mtl(m){}
	std::vector<face_triple> data;
	material* mtl;
};
typedef std::string t_group;
typedef int s_group;

std::vector<tex_coords> *texture_coordinates;
std::vector<vnormal> *normals;
std::vector<t_vertex> *vertices;
std::vector<t_face> *faces;

t_materials mat_map;

material* current_material=new material;

union yystype{
	face_triple*	ft;
	t_face*     	face;
	double		d;
	char *		str;
};

#define YYSTYPE yystype

int OBJ_PARSEregel=1;

extern "C" void yyerror(const char *str){
	    fprintf(stderr,"YACC:error: %s\n",str);
	    fprintf(stderr,"YACC:error: on line %i\n",OBJ_PARSEregel);
}
 
extern "C" int OBJ_PARSEwrap(){
	OBJ_PARSEregel=0;
	return 1;
} 

%}
%token TEXTURE_VERTEX VERTEX_NORMAL VERTEX FACE GROUP SMOOTHING_GROUP OBJECT MTLLIB USEMTL NUM NAME NL SLASH WS COMMENT CONTINUATION

%%
start	: file					
	;

file	: file NL line			{OBJ_PARSEregel++;}
		| line			{OBJ_PARSEregel++;}
		;

line	: 		tex_vertex
		|	VERTEX_NORMAL 		NUM NUM NUM		{normals->push_back(vnormal($2.d,$3.d,$4.d));}
		|	VERTEX 			NUM NUM NUM 	{vertices->push_back(t_vertex($2.d,$3.d,$4.d));}
		|	face
		|	GROUP 			NAME 
		|	SMOOTHING_GROUP 		NUM
		|	OBJECT 			NAME
		|	MTLLIB 			NAME			{
											t_materials mats=mtl_parse($2.str);
											mat_map.insert(mats.begin(), mats.end());
										
										}
		|	USEMTL			NAME			{current_material=&(mat_map)[$2.str];}
		|   /*  empty */
		;

face	: FACE face_data_list				{faces->push_back(*$2.face);}
		;

face_data_list	: face_data_list face_data			{$$=$1;$1.face->data.push_back(*$2.ft);}
				| 						{$$.face=new t_face(current_material);}
				;

face_data	: NUM SLASH 	SLASH NUM				{$$.ft=new face_triple($1.d-1,-1,$4.d-1);}
			| NUM SLASH NUM 					{$$.ft=new face_triple($1.d-1,$3.d-1);}
			| NUM SLASH NUM SLASH NUM			{$$.ft=new face_triple($1.d-1,$3.d-1,$5.d-1);}
			| NUM							{$$.ft=new face_triple($1.d-1);}
			;


tex_vertex	: TEXTURE_VERTEX NUM				{texture_coordinates->push_back(vnormal($2.d,-1,-1));}
			| TEXTURE_VERTEX NUM NUM		{texture_coordinates->push_back(vnormal($2.d,$3.d,-1));}
			| TEXTURE_VERTEX NUM NUM NUM		{texture_coordinates->push_back(vnormal($2.d,$3.d,$4.d));}
			;














		
