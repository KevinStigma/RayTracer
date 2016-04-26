/*    
 *	This header file defines objects inheritance framework used for ray tracing,
 *  but you can also use it for other fileds.
 *  Yuke Zhu
 *  kevinstigma@gmail.com
 */
#ifndef OBJECTS_H
#define OBJECTS_H
#include "Data_structure.h"
#include "RandomGenerator.h"

struct GLMmodel;
namespace zyk
{
	class Object
	{
	public:
		Object():m_material(NULL){}
		virtual ~Object();
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const=0;
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t)const=0;
		void setMaterial(Material* mat) {assert(mat);m_material=mat;}
		const Material* getMaterial()const {return m_material;}
	protected:
		Material* m_material;
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
		float d; // nx+d=0
		Vec3 n;
	};

	struct OBB
	{
		Vec3 bot_pos;
		Vec3 top_pos;
		float XL,YL,ZL;
		Vec3 local_coord[3];
		OBB(Vec3 b_pos=Vec3(0,0,0),Vec3 t_pos=Vec3(0,0,0)):bot_pos(b_pos),top_pos(t_pos)
		{
			XL=top_pos(0)-bot_pos(0);
			YL=top_pos(1)-bot_pos(1);
			ZL=top_pos(2)-bot_pos(2);
		}
		Vec3 getRandomPt()const//get random point in the OBB framework
		{
			static RandomGenerator rand_gen(0);
			float xl=rand_gen.getRand()*XL,yl=rand_gen.getRand()*YL,zl=rand_gen.getRand()*ZL;
			return bot_pos+local_coord[0]*xl+local_coord[1]*yl+local_coord[2]*zl;
		}
	};

	struct AABB
	{
		Vec3 bot_pos;
		Vec3 top_pos;
		float XL,YL,ZL;
		inline bool inBox(const Vec3&point)const;
		AABB(Vec3 b_pos=Vec3(0,0,0),Vec3 t_pos=Vec3(0,0,0)):bot_pos(b_pos),top_pos(t_pos)
		{
			XL=top_pos(0)-bot_pos(0);
			YL=top_pos(1)-bot_pos(1);
			ZL=top_pos(2)-bot_pos(2);
		}
		bool intersectCheck(const Vec3& origin,const Vec3& dir)const;
	};

	class TriMesh:public Object
	{
	public:
		TriMesh(const std::string& name);
		TriMesh();
		~TriMesh();
		bool importMesh(const std::string&filename);
		void scaleMesh(const Vec3& scale);
		Vec3 getCenter();
		int getVertexNum();
		void translate(const Vec3& trans);
		void calOBB();
		void calAABB();
		void reverseNormals();
		void setNormal(float nx,float ny,float nz);//we set all the vertices a same normal for some special case
		const GLMmodel* getMesh()const{return mModel;}
		GLMmodel* getMesh(){return mModel;}
		const AABB* getAABB()const{return m_aabb;}
		const OBB* getOBB()const{return m_obb;}
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const;
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t)const;
	protected:
		void calVertNormal(int status=1);//calculate the mesh's normals for each vertex, status means the direction of normal
	private:
		GLMmodel* mModel;
		Vec3 m_center;
		AABB* m_aabb;
		OBB*  m_obb;
	};

	class Triangle:public Object
	{
	public:
		Triangle(Vec3 triangle_pt[]);
		~Triangle();
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const;
		virtual bool intersect(const Vec3& origin,const Vec3& dir,float& t)const;
		void setPoints(Vec3 triangle_pt[]);
		void reverseNormal();
		Vec3 mNormal;
	private:
		Vec3 tri_pt[3];
	};
};
#endif