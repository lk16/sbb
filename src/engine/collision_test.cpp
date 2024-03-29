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
	cross_points_list res;
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

bool collide(const flomath::polygon& t1,const flomath::polygon& t2){
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



