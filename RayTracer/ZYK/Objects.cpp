#include "Objects.h"
#include "mathematics.h"
#include "glm.h"

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
		float tmp=dir.dot(n);
		if(FCMP(tmp,0))
			return false;
		t=-(origin.dot(n)+d)/tmp;
		if(t>0)
			return true;
		else 
			return false;
	}

	TriMesh::TriMesh(const std::string& name):mModel(NULL),m_aabb(NULL),m_obb(NULL)
	{
		if(!importMesh(name))
			mModel=NULL;
	}

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
				normal=(1-coord_para(0)-coord_para(1))*vNormal[0]+
					coord_para(0)*vNormal[1]+coord_para(1)*vNormal[2];
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

	void TriMesh::calOBB()
	{
		if(!mModel||mModel->numvertices==0)
			return;
		if(m_obb)
			return;
		//using Principle Component Analysis to get the local coordinate of the mesh
		Vec3 eigenValues;
		Mat3 eigenVectors;
		PCAfor3D(mModel->vertices+3,mModel->numvertices,eigenValues,eigenVectors);
		Vec3 m_local_coord[3];
		for(int i=0;i<2;i++)
		{
			m_local_coord[i]=eigenVectors.col(i);
			m_local_coord[i].normalize();
		}
		m_local_coord[2]=m_local_coord[0].cross(m_local_coord[1]);
		m_local_coord[1]=m_local_coord[0].cross(m_local_coord[2]);
		m_obb=new OBB;
		for(int i=0;i<2;i++)
			m_obb->local_coord[i]=m_local_coord[i];

		//we then compute the length of mesh in each local coordinate
		float local_xmin,local_xmax,local_ymax,local_ymin,local_zmax,local_zmin;
		Mat3 trans_mat;
		for(int i=0;i<3;i++)
		{
			trans_mat(0,i)=m_local_coord[i](0);
			trans_mat(1,i)=m_local_coord[i](1);
			trans_mat(2,i)=m_local_coord[i](2);
		}
		trans_mat=trans_mat.inverse();

		Vec3 trans_pt=trans_mat*Vec3(mModel->vertices[3],mModel->vertices[4],mModel->vertices[5]);
		local_xmin=local_xmax=trans_pt(0);
		local_ymin=local_ymax=trans_pt(1);
		local_zmin=local_zmax=trans_pt(2);
		for(int i=2;i<=mModel->numvertices;i++)
		{
			int index=i*3;
			trans_pt=trans_mat*Vec3(mModel->vertices[index],
				mModel->vertices[index+1],mModel->vertices[index+2]);
			local_xmin=std::min(local_xmin,trans_pt(0));
			local_ymin=std::min(local_ymin,trans_pt(1));
			local_zmin=std::min(local_zmin,trans_pt(2));

			local_xmax=std::max(local_xmax,trans_pt(0));
			local_ymax=std::max(local_ymax,trans_pt(1));
			local_zmax=std::max(local_zmax,trans_pt(2));
		}
		m_obb->XL=(local_xmax-local_xmin)*0.5f;
		m_obb->YL=(local_ymax-local_ymin)*0.5f;
		m_obb->ZL=(local_zmax-local_zmin)*0.5f;

		trans_mat=trans_mat.inverse();
		m_obb->bot_pos=trans_mat*Vec3(local_xmin,local_ymin,local_zmin);
		m_obb->top_pos=trans_mat*Vec3(local_xmax,local_ymax,local_zmax);
	}

	void TriMesh::calAABB()
	{
		if(!mModel||mModel->numvertices==0)
			return;
		if(m_aabb)
			return;
		m_aabb=new AABB;

		float xmin,xmax,ymax,ymin,zmax,zmin;
		xmin=xmax=mModel->vertices[3];
		ymin=ymax=mModel->vertices[4];
		zmin=zmax=mModel->vertices[5];
		for(int i=2;i<=mModel->numvertices;i++)
		{
			int index=i*3;
			Vec3 point(mModel->vertices[index],mModel->vertices[index+1],mModel->vertices[index+2]);
			xmin=std::min(xmin,point(0));
			xmax=std::max(xmax,point(0));
			ymin=std::min(ymin,point(1));
			ymax=std::max(ymax,point(1));
			zmin=std::min(zmin,point(2));
			zmax=std::max(zmax,point(2));
		}
		m_aabb->XL=(xmax-xmin)*0.5f;
		m_aabb->YL=(ymax-ymin)*0.5f;
		m_aabb->ZL=(zmax-zmin)*0.5f;
		m_aabb->bot_pos=Vec3(xmin,ymin,zmin);
		m_aabb->top_pos=Vec3(xmax,ymax,zmax);
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
		SAFE_DELETE(m_aabb);
		SAFE_DELETE(m_obb);
	}

	Triangle::Triangle(Vec3 triangle_pt[])
	{
		setPoints(triangle_pt);
		mNormal=(tri_pt[1]-tri_pt[0]).cross(tri_pt[2]-tri_pt[1]);
		mNormal.normalize();
	}

	inline void Triangle::reverseNormal(){mNormal=-mNormal;}
	Triangle::~Triangle(){}

	bool Triangle::intersect(const Vec3& origin,const Vec3& dir,float& t,Vec3& normal,Vec3& intersect_pt)const
	{
		if(intersect(origin,dir,t))
		{
			intersect_pt=origin+t*dir;
			normal=mNormal;
			return true;
		}
		return false;
	}

	bool Triangle::intersect(const Vec3& origin,const Vec3& dir,float& t)const
	{
		Vec2 coord_para;
		if(tri_intersect_test(origin,dir,tri_pt,t,coord_para)&&t>0)
			return true;
		return false;
	}

	inline void Triangle::setPoints(Vec3 triangle_pt[])
	{
		for(int i=0;i<3;i++)
			tri_pt[i]=triangle_pt[i];
	}

	struct CandidatePlane// this data structure is only used for AABB::intersectCheck
	{
		Plane3D* plane;
		float t;
		CandidatePlane(Plane3D*p=NULL,float t1=0):plane(p),t(t1){}
	};

	inline bool AABB::inBox(const Vec3&point)const
	{
		if(point(0)>=bot_pos(0)&&point(0)<=top_pos(0)&&
			point(1)>=bot_pos(1)&&point(1)<=top_pos(1)&&
			point(2)>=bot_pos(2)&&point(2)<=top_pos(2))
			return true;
		return false;
	}

	bool AABB::intersectCheck(const Vec3& origin,const Vec3& dir,float& t)const
	{
		static Plane3D planes[6]={Plane3D(Vec3(1,0,0),top_pos),Plane3D(Vec3(1,0,0),bot_pos),
			Plane3D(Vec3(0,1,0),top_pos),Plane3D(Vec3(0,1,0),bot_pos),
			Plane3D(Vec3(0,0,1),top_pos),Plane3D(Vec3(0,0,1),bot_pos)};

		std::vector<CandidatePlane> near_plane;

		//select the candidate planes
		for(int i=0;i<3;i++)
		{	
			float t1,t2;
			bool is1=planes[i*2].intersect(origin,dir,t1);
			bool is2=planes[i*2+1].intersect(origin,dir,t2);
			if((is1&&!is2)||(is1&&is2&&t1<t2))
				near_plane.push_back(CandidatePlane(&planes[i*2],t1));
			else if((!is1&&is2)||((is1&&is2&&!(t1<t2))))
				near_plane.push_back(CandidatePlane(&planes[i*2+1],t2));
		}
		if(near_plane.size()==0)
		{
			t=0;
			return false;
		}

		std::sort(near_plane.begin(),near_plane.end(),
			[](const CandidatePlane&a,const CandidatePlane&b){return a.t>b.t;});
		//find the farthest to the ray origin in candidate planes
		for(int i=0;i<(int)near_plane.size();i++)
		{
			Vec3 inter_pt=origin+dir*near_plane[i].t;
			//we must guarantee that the intersect point is in the field of AABB
			if(inBox(inter_pt))
			{
				t=near_plane[i].t;
				return true;
			}
		}
		return false;
	}
};