#ifndef OBJECTS_H
#define OBJECTS_H
#include "Data_structure.h"
namespace zyk
{
	class Object
	{
	public:
		Object():m_material(NULL){}
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const=0;
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t)const=0;
		void setMat(Material_Ptr mat) {m_material=mat;}
		const Material_Ptr getMat()const {return m_material;}
	protected:
		Material_Ptr m_material;
	};
	
	class Sphere:public Object
	{
	public:
		Sphere(Vec3 cent=Vec3(0,0,0),float r=0):center(cent),radius(r){}
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const;
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t)const;
		Vec3 center;
		float radius;
	};
};
#endif