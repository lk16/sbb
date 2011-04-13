#include "figure.hpp"
#include "object-parser/parse.hpp"
#include "flomath.hpp"

#include <cassert>

figure::figure(t_engine& e,const flomath::point& pos, flomath::base_figure data):
	engine_interface(e,ei_drawable),
	flomath::base_figure(data)
{
	*static_cast<point*>(this)=pos;
}
	
figure::figure(t_engine& e,double _x,double _y,double _z):
		engine_interface(e,ei_drawable)
{
	*static_cast<flomath::point*>(this) = point(_x,_y,_z);
}

figure::figure(t_engine& e,const flomath::point& pos):
	engine_interface(e,ei_drawable)
{
	*static_cast<point*>(this)=pos;
}

unsigned figure::get_texture_tag(const std::string& fname){

	Glib::RefPtr<Gdk::Pixbuf> pic=Gdk::Pixbuf::create_from_file(fname);
	
	char* data=reinterpret_cast<char*>(pic->get_pixels());
	size_t rs=pic->get_rowstride();
	size_t wi=pic->get_width();
	
	std::vector<char> buff;
	for(int j=0;j<pic->get_height();j++){
		buff.insert(buff.end(),data+rs*j,data+rs*j+wi*pic->get_n_channels());
	}
	GLuint format=pic->get_has_alpha()?GL_RGBA:GL_RGB;
	assert(pic->get_bits_per_sample()==8);

	GLuint texname;
	
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1,&texname);
	glBindTexture(GL_TEXTURE_2D,texname);

	assert(texname);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, pic->get_width(),
	pic->get_height(), 0, format, GL_UNSIGNED_BYTE, &buff[0]);
	return texname;
}

void figure::file2fig(std::string fname){
	const obj_file& file=obj_parse(fname);
	bool has_alpha=false;
	typedef std::vector<t_face>::const_iterator iter;
	for(iter i=file.faces.begin();i!=file.faces.end();++i){
		drawable_face here;
		add_face(*i,file);
		if(i->mtl->filename){
			here.has_texture=true;
			here.texname = get_texture_tag((obj_folder/i->mtl->filename).string());
			typedef std::vector<face_triple>::const_iterator inner_iter;
			for(inner_iter j=i->data.begin();j!=i->data.end();j++){
				drawable_face::coord c;
				static_cast<flomath::point&>(c)=file.vertices[j->vert_ind];
				assert(j->tex_ind>=0); //you asshole forgot the tex-coords for a plane which requires them
				c.tex=file.texture_coordinates[j->tex_ind];
				here.coords.push_back(c);
			}
		}
		else{
			here.has_texture=false;
			here.alpha=i->mtl->alpha;
			here.r=i->mtl->kd.r;
			here.g=i->mtl->kd.g;
			here.b=i->mtl->kd.b;
			typedef std::vector<face_triple>::const_iterator inner_iter;
			for(inner_iter j=i->data.begin();j!=i->data.end();j++){
				drawable_face::coord c;
				static_cast<flomath::point&>(c)=file.vertices[j->vert_ind];
				here.coords.push_back(c);
			}
			
		}
		if(!here.has_texture && !flomath::equals(here.alpha,1)){
			has_alpha = true;
		}
		draw_faces.push_back(here);
	}
	if(has_alpha){
		remove_interface(ei_drawable);
		e->transparent_drawables.add(this);
	}
}

void figure::add_face(const t_face& face,const obj_file& file){
	if(face.data.size()<3){
		std::cerr << "error: too few points for a triangle/polygon" << std::endl;
		std::exit(0);
	}
	flomath::polygon pol;
	for(unsigned i=0;i<face.data.size();i++){
		pol.p.push_back(file.vertices[face.data[i].vert_ind]);
	}
	polly.push_back(pol);
}

figure::figure(t_engine& e, double _x,double _y,double _z,std::string fname):
	engine_interface(e,ei_drawable|ei_has_texture),
	file(fname)
{
	*static_cast<flomath::point*>(this) = point(_x,_y,_z);
}

figure::figure(t_engine& e,const flomath::point& pos,std::string fname):
	engine_interface(e,ei_drawable|ei_has_texture),
	file(fname)
{
	*static_cast<flomath::point*>(this) = pos;
}

void figure::load_textures()
{
	if(file!=""){
		file2fig(file);
	}
}

///draw the 'draw' model
void figure::draw(){
	glPushMatrix();
	glTranslated(x,y,z);
	glRotateq(rotation);
	glBegin(GL_POLYGON);
	for(std::vector<drawable_face>::iterator i=draw_faces.begin();i!=draw_faces.end();++i){
		i->draw();
	}
	glEnd();
	glPopMatrix();
}

///draw the collision model!
void figure::draw_col_fig()
{
	glPushMatrix();
	glTranslated(x,y,z);
	glRotateq(rotation);
	glBegin(GL_POLYGON);
	for(std::vector<flomath::polygon>::iterator i=polly.begin();i!=polly.end();++i){
		for(std::vector<flomath::point>::iterator j=i->p.begin();j!=i->p.end();j++){
			glVertex3d(j->x,j->y,j->z);
		}
	}
	glEnd();
	glPopMatrix();
}

void drawable_face::draw(){
	if(has_texture){
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glBindTexture(GL_TEXTURE_2D,texname);
		glColor4d(1,1,1,1);
		glBegin(GL_POLYGON);
		for(std::vector<coord>::iterator i=coords.begin();i!=coords.end();i++){
			glTexCoord2d(i->tex.x,i->tex.y);
			glVertex3d(i->x,i->y,i->z);
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	else{
		if(flomath::equals(alpha,0)){
			return;
		}
		glColor4d(r,g,b,alpha);
		glBegin(GL_POLYGON);
		for(std::vector<coord>::iterator i=coords.begin();i!=coords.end();i++){
			glVertex3d(i->x,i->y,i->z);
		}
		glEnd();
	}
}

