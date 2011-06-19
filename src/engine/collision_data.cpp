#include "collision_data.hpp"

void collision_data_base::swap(){
	flomath::polygon* tmp=p_other;
	p_other=p_mine;
	p_mine=tmp;
}

collision_data_base* collide(figure& f1,figure& f2){
	if(flomath::distance(f1,f2)>f1.radius()+f2.radius()){
		return NULL;
	}
	using flomath::polygon;
	for(std::vector<polygon>::iterator i=f1.polly.begin();i!=f1.polly.end();i++){
		for(std::vector<polygon>::iterator j=f2.polly.begin();j!=f2.polly.end();j++){
			polygon a(*i), b(*j);
			a.rotate(f1.rotation);
			b.rotate(f2.rotation);
			a+=f1;
			b+=f2;
			if(collide(a,b)) return new collision_data_base(&*i,&*j);
		}
	}
	return NULL;
}

// helping function for collide(base_sphere&,figure&);
bool sphere_collides_line(flomath::point& swrtf,double r,const flomath::point& p1,const flomath::point& p2){
	flomath::line3d l(p1,p2);
	double d = flomath::distance(swrtf,l);
	if(d<r){
		flomath::point p = flomath::project(swrtf,l);
		flomath::vector a = p-p1;
		flomath::vector b = p-p2;
		double g = flomath::dotproduct(a,b)/a.length()/b.length();
		if(flomath::equals(g,-1) || flomath::distance(swrtf,p1)<r){
			return true;
		}
	}
	return false;
}

collision_data_base* collide(figure& f,base_sphere& s){
	collision_data_base* res=collide (s,f);
	res->swap();
	return res;
}

collision_data_base* collide(base_sphere& s,figure& f){
	if((f.radius()+s.radius())<flomath::distance(s,f)){ 
		return NULL;
	}
	flomath::point swrtf(s-f);
	swrtf.rotate(-f.rotation.conj());
	
	for(unsigned i=0;i<f.polly.size();i++){
		flomath::plane pl(f.polly[i]);
		if(flomath::abs(flomath::distance(swrtf,pl))<s.radius()){
			if(f.polly[i].is_in_polygon(swrtf)){
				return new collision_data_base(NULL,&f.polly[i]);
			}
			flomath::point p0 = f.polly[i].p.front(); 
			p0.rotate(f.rotation);
			flomath::point p1;
			
			for(std::vector<flomath::point>::const_iterator k=f.polly[i].p.begin()+1;k!=f.polly[i].p.end();k++){
				p1 = *k;
					if(sphere_collides_line(swrtf,s.radius(),p0,p1)){
					return new collision_data_base(NULL,&f.polly[i]);
				}
				p0 = p1;
			}
			if(sphere_collides_line(swrtf,s.radius(),f.polly[i].p.back(),f.polly[i].p.front())){
				return new collision_data_base(NULL,&f.polly[i]);
			}
		}
	}
	return NULL;
}

collision_data_base* collide(base_sphere& s1,base_sphere& s2){
	return flomath::distance(s1,s2) < s1.radius() + s2.radius() ? new collision_data_base(NULL,NULL) : NULL ;
}


