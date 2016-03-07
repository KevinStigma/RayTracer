/*    
 *	This header file defines objects inheritance framework used for ray tracing,
 *  but you can also use it for other fileds.
 *  Yuke Zhu
 *  kevinstigma@gmail.com
 */
#ifndef OBJECTS_H
#define OBJECTS_H
#include "Data_structure.h"
#include "glm.h"

namespace zyk
{
	class Object
	{
	public:
		Object():m_material(NULL){}
		virtual ~Object(){}
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const=0;
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t)const=0;
		void setMaterial(Material_Ptr mat) {m_material=mat;}
		const Material_Ptr getMaterial()const {return m_material;}
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


	class Plane3D:public Object
	{
	public:
		Plane3D();
		Plane3D(const Vec3&pNormal,const Vec3&pPoint);
		void setPlan3D(const Vec3& pNormal,const Vec3 &pPoint);
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const;
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t)const;
		float d;
		Vec3 n;
	};

	class TriMesh:public Object
	{
	public:
		TriMesh();
		~TriMesh();
		bool importMesh(const std::string&filename);
		void scaleMesh(const Vec3& scale);
		Vec3 getCenter();
		void translate(const Vec3& trans);
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const;
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t)const;
	protected:
		void calVertNormal(int status=1);
	private:
		GLMmodel* mModel;
		Vec3 m_center;
	};
};
#endif