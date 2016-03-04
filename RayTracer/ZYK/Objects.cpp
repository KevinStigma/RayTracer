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

	TriMesh::TriMesh():mModel(NULL)
	{

	}

	bool TriMesh::intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const 
	{
		return true;
	}
	bool TriMesh::intersect(const Vec3& origin,const Vec3& dir,float& t)const
	{
		return true;
	}

	void TriMesh::importMesh(const std::string&filename)
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
			return;
		}
		m_center=Vec3(0,0,0);
		for(int i=1;i<=mModel->numvertices;i++)
			m_center+=Vec3(mModel->vertices[i*3],mModel->vertices[i*3+1],mModel->vertices[i*3+2]);
		m_center/=mModel->numvertices;
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

	TriMesh::~TriMesh()
	{
		if (mModel)
		{
			glmDelete( mModel );
			mModel = NULL;
		}
	}
};