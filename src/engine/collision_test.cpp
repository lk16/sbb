#include "collision_test.hpp"

bool debug=false;

/* BUG er is een marge 2 waarop hij toch collision meldt.. 
*  bij doorkruising van het p van de andere driehoek 
*  en een afstand in het p van die andere driehoek van ongeveer .5
*  meldt hij nog steeds een collision
*/
typedef std::vector<std::pair<flomath::point,flomath::point> > cross_points_list;

cross_points_list collide_find_cross_points(flomath::plane p,const std::vector<flomath::point>&points)
{
	std::vector<std::pair<flomath::point,flomath::point> > res;
	typedef std::vector<flomath::point>::const_iterator point_iter;
	for(point_iter i=points.begin();i!=points.end()-1;i++){
		double first = p.eval(*i);
		double second = p.eval(*(i+1));
		if(first<=0 ^ second<=0){
			res.push_back(std::make_pair(*i,*(i+1)));
		}
	}
	return res;
}

void collision_data_base::swap(){
	flomath::polygon* tmp=p_other;
	p_other=p_mine;
	p_mine=tmp;
}

bool collide(flomath::polygon t1,flomath::polygon t2){
	using flomath::abs;
	
	cross_points_list buur_list1=collide_find_cross_points(t1,t1.p);
	cross_points_list buur_list2=collide_find_cross_points(t2,t2.p);
	
	if(buur_list1.empty() || buur_list2.empty()){
		return false;
	}
	
	flomath::line3d l = flomath::get_intersect_line(t1,t2);
	double flomath::point::* c;
	if(abs(l.rc.x)>abs(l.rc.y))
		if(abs(l.rc.x)>abs(l.rc.z))
			c=&flomath::point::x;
		else
			c=&flomath::point::z;
	else if(abs(l.rc.y)>abs(l.rc.z))
		c=&flomath::point::y;
	else
		c=&flomath::point::z;

	
	std::vector<double> line_coords_a, line_coords_b;
	
	for(int i=0;i<buur_list1.size();i++){
		flomath::line3d cur_line(buur_list1[i].first,buur_list1[i].second);
		line_coords_a.push_back(flomath::intersect_lines(l,cur_line).*c);
	}
	
	for(int i=0;i<buur_list2.size();i++){
		flomath::line3d cur_line(buur_list2[i].first,buur_list2[i].second);
		line_coords_b.push_back(flomath::intersect_lines(l,cur_line).*c);
	}
	
	std::sort(line_coords_a.begin(),line_coords_a.end());
	std::sort(line_coords_b.begin(),line_coords_b.end());
	

	std::vector<double>::iterator coord_iter[2]={
		line_coords_a.begin(),
		line_coords_b.begin()
	};

	int length = 1,string_owner=-1;

	do{
		bool winner=*coord_iter[0]<*coord_iter[1];
		
		if(string_owner==winner){
			length++;
		}
		else{
			if(length%2){
				
				return true;
			}
			length=1;
			string_owner=winner;
		}
		++coord_iter[winner];
	}while(coord_iter[0]!=line_coords_a.end()  &&  coord_iter[1]!=line_coords_b.end());
	return false;
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


collision_data_base* collide(figure& f,base_sphere& s){
	collision_data_base* res=collide (s,f);
	res->swap();
	return res;
}

// hulp functie voor collide(base_sphere&,figure&);
bool sphere_collides_line(flomath::point& swrtf,double r,const flomath::point& p1,const flomath::point& p2){
	flomath::line3d l(p1,p2);
	double d = flomath::distance(swrtf,l);
	if(d<r){
		flomath::point p = flomath::project(swrtf,l);
		flomath::vector a = p-p1;
		flomath::vector b = p-p2;
		double g = flomath::dotproduct(a,b)/a.length()/b.length();
		if(flomath::equals(g+1,0) || flomath::distance(swrtf,p1)<r){
			return true;
		}
	}
	return false;
}

collision_data_base* collide(base_sphere& s,figure& f){
	if((f.radius()+s.r)<flomath::distance(s,f)){ 
		return NULL;
	}
	flomath::point swrtf(s-f);
	swrtf.rotate(-f.rotation.conj());
	
	for(unsigned i=0;i<f.polly.size();i++){
		flomath::plane pl(f.polly[i]);
		if(flomath::abs(flomath::distance(swrtf,pl))<s.r){
			if(f.polly[i].is_in_polygon(swrtf)){
				return new collision_data_base(NULL,&f.polly[i]);
			}
			flomath::point p0 = f.polly[i].p.front(); 
			p0.rotate(f.rotation);
			flomath::point p1;
			
			for(std::vector<flomath::point>::const_iterator k=f.polly[i].p.begin()+1;k!=f.polly[i].p.end();k++){
				p1 = *k;
					if(sphere_collides_line(swrtf,s.r,p0,p1)){
					return new collision_data_base(NULL,&f.polly[i]);
				}
				p0 = p1;
			}
			if(sphere_collides_line(swrtf,s.r,f.polly[i].p.back(),f.polly[i].p.front())){
				return new collision_data_base(NULL,&f.polly[i]);
			}
		}
	}
	return NULL;
}

collision_data_base* collide(base_sphere& s1,base_sphere& s2){
	return flomath::distance(s1,s2) < s1.r + s2.r ? new collision_data_base(NULL,NULL) : NULL ;
}


