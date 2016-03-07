#include "Objects.h"
#include "mathematics.h"

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

	Plane3D::Plane3D():d(0),n(0,0,0){}

	Plane3D::Plane3D(const Vec3&pNormal,const Vec3&pPoint)
	{
		setPlan3D(pNormal,pPoint);
	}

	void Plane3D::setPlan3D(const Vec3& pNormal,const Vec3 &pPoint)
	{
		n=pNormal.normalized();
		d=-pPoint.dot(n);
	}

	bool Plane3D::intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const
	{
		if(!intersect(origin,dir,t))
			return false;
		intersect_pt=origin+dir*t;
		normal=n;
		return true;
	}

	bool Plane3D::intersect(const Vec3& origin,const Vec3& dir,float& t)const
	{
		t=-(origin.dot(n)+d)/dir.dot(n);
		if(t>0)
			return true;
		else 
			return false;
	}

	TriMesh::TriMesh():mModel(NULL){}

	bool TriMesh::intersect(const Vec3& origin,const Vec3& ray_dir,float& t,Vec3& normal,Vec3& intersect_pt)const 
	{
		Vec3 vVert[3],vNormal[3];
		Vec2 coord_para;
		t=0;
		for(int i=0;i<mModel->numtriangles;i++)
		{
			for(int j=0;j<3;j++)
			{
				int index=mModel->triangles[i].vindices[j];
				vVert[j]=Vec3(mModel->vertices[index*3],mModel->vertices[index*3+1],mModel->vertices[index*3+2]);
				vNormal[j]=Vec3(mModel->normals[index*3],mModel->normals[index*3+1],mModel->normals[index*3+2]);
			}
			if(tri_intersect_test(origin,ray_dir,vVert,t,coord_para)&&t>0)
			{
				intersect_pt=origin+t*ray_dir;
				//normal interpolation
				normal=(1-coord_para(0)-coord_para(1))*vNormal[0]+coord_para(0)*vNormal[1]+coord_para(1)*vNormal[2];
				return true;
			}
		}
		return false;
	}

	bool TriMesh::intersect(const Vec3& origin,const Vec3& ray_dir,float& t)const
	{
		Vec3 vVert[3];
		Vec2 coord_para;
		t=0;
		for(int i=0;i<mModel->numtriangles;i++)
		{
			for(int j=0;j<3;j++)
			{
				int index=mModel->triangles[i].vindices[j];
				vVert[j]=Vec3(mModel->vertices[index*3],mModel->vertices[index*3+1],mModel->vertices[index*3+2]);
			}
			if(tri_intersect_test(origin,ray_dir,vVert,t,coord_para)&&t>0)
				return true;
		}
		return false;
	}

	bool TriMesh::importMesh(const std::string&filename)
	{
		if (mModel)
		{
			glmDelete(mModel);
			mModel = NULL;
		}
		mModel = glmReadOBJ(filename.c_str());
		if(!mModel)
		{
			std::cout<<"Can't read the mesh"<<std::endl;
			return false;
		}
		m_center=Vec3(0,0,0);
		for(int i=1;i<=mModel->numvertices;i++)
			m_center+=Vec3(mModel->vertices[i*3],mModel->vertices[i*3+1],mModel->vertices[i*3+2]);
		m_center/=mModel->numvertices;
		calVertNormal(1);
		return true;
	}

	void TriMesh::calVertNormal(int status)
	{
		if(!mModel)
			return;
		std::vector<std::vector<int>> vert_2_face(mModel->numvertices+1);
		std::vector<Vec3> face_normal(mModel->numtriangles);
		Vec3 vert[3];
		const float *vertices=mModel->vertices;
		for(int i=0;i<mModel->numtriangles;i++)
		{
			for(int j=0;j<3;j++)
			{
				int index=mModel->triangles[i].vindices[j];
				vert[j]=Vec3(vertices[index*3],vertices[index*3+1],vertices[index*3+2]);
				vert_2_face[index].push_back(i);
			}

			assert(status==1||status==2);
			Vec3 nor;
			if(status==1)
				nor=((vert[1]-vert[0]).cross(vert[2]-vert[1])).normalized();
			else if(status==2)
				nor=-((vert[1]-vert[0]).cross(vert[2]-vert[1])).normalized();

			face_normal[i]=nor;
		}

		mModel->numnormals=mModel->numvertices;
		mModel->normals=(float*)malloc(sizeof(GLfloat)*3*(mModel->numnormals+1));
		for(int i=1;i<=mModel->numvertices;i++)
		{
			Vec3 avg_nor(0,0,0);
			for(int j=0;j<(int)vert_2_face[i].size();j++)
			{
				avg_nor+=face_normal[vert_2_face[i][j]];
			}
			if(!vert_2_face[i].size())
				continue;
			avg_nor/=vert_2_face[i].size();
			mModel->normals[3*i]=avg_nor[0];
			mModel->normals[3*i+1]=avg_nor[1];
			mModel->normals[3*i+2]=avg_nor[2];
		}
	}

	void TriMesh::scaleMesh(const Vec3& scale)
	{
		if(!mModel)
			return;
		for(int i=1;i<=mModel->numvertices;i++)
			for(int j=0;j<3;j++)
				mModel->vertices[i*3+j]= (mModel->vertices[i*3+j]-m_center(j))*scale(j)+m_center(j);
	}

	Vec3 TriMesh::getCenter()
	{
		if(!mModel)
		{
			std::cout<<"You don't generate model now!"<<std::endl;
			return Vec3(0,0,0);
		}
		return m_center;
	}

	void TriMesh::translate(const Vec3& trans)
	{
		if(!mModel)
			return;
		for(int i=1;i<=mModel->numvertices;i++)
		{
			int ind=i*3;
			mModel->vertices[ind]+=trans[0];
			mModel->vertices[ind+1]+=trans[1];
			mModel->vertices[ind+2]+=trans[2];
		}
	}

	TriMesh::~TriMesh()
	{
		if (mModel)
		{
			glmDelete( mModel );
			mModel = NULL;
		}
	}
};