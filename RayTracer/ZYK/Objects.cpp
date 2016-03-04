#include "Objects.h"

namespace zyk
{
	bool Sphere::intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& p_normal,Vec3& inters_pt)const
	{
		if(!intersect(origin,dir,t))
			return false;
		inters_pt=origin+t*dir;
		p_normal=(inters_pt-center).normalized();
		return true;
	}

	bool Sphere::intersect(const Vec3& origin,const Vec3& dir,float& t)const
	{
		t=0;
		float tmp_val=dir.dot(origin-center);
		Vec3 tmp_vec=origin-center;
		float val=tmp_val*tmp_val-dir.dot(dir)*(tmp_vec.dot(tmp_vec)-radius*radius);

		float inv_dot=1/dir.dot(dir);
		if(val<0)
			return false;
		else if(FCMP(val,0.0))
		{
			t=-tmp_val*inv_dot;
			if(t<0)
				return false;
		}
		else
		{
			val=sqrt(val);
			float t1=(-tmp_val+val)*inv_dot;
			float t2=(-tmp_val-val)*inv_dot;
			if(t1<0&&t2<0)
				return false;
			else if(t1<0)
				t=t2;
			else if(t2<0)
				t=t1;
			else
				t=std::min(t1,t2);
		}
		return true;
	}
};