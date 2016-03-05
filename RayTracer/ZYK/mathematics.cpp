#include "mathematics.h"
namespace zyk
{
	QUAT QUAT::Identify=QUAT(1,0,0,0);
	Vec3 QUAT::rotate(float angle/*radian*/,const Vec3& axis,const Vec3& point)
	{
		float cos_v=cos(angle*0.5f);
		float sin_v=sin(angle*0.5f);
		QUAT vq(0,point(0),point(1),point(2));
		QUAT rot(cos_v,sin_v*axis[0],sin_v*axis[1],sin_v*axis[2]);
		rot.normalize();
		QUAT vq2=rot.conjugate()*vq*rot;//counterclockwise in right hand coordinate
		return Vec3(vq2.m(1),vq2.m(2),vq2.m(3));
	}

	Vec4 QUAT::rotate(float angle,const Vec3& axis,const Vec4&point)
	{
		Vec3 p(point(0),point(1),point(2));
		Vec3 rp=rotate(angle,axis,p);
		return Vec4(rp(0),rp(1),rp(2),1.f);
	}
	inline QUAT QUAT::operator+(const QUAT&q)
	{
		return QUAT(m+q.m);
	}

	inline QUAT QUAT::operator-(const QUAT&q)
	{
		return QUAT(m-q.m);
	}

	inline QUAT QUAT::operator*(const QUAT&q)
	{
		Mat4 mat;
		mat<<m(3),-m(2),m(1),m(0),
			m(2),m(3),-m(0),m(1),
			-m(1),m(0),m(3),m(2),
			-m(0),-m(1),-m(2),m(3);
		return QUAT(mat*q.m);
	}

	inline QUAT QUAT::operator*(const float&val)
	{
		return QUAT(val*m);
	}

	inline QUAT QUAT::operator=(const QUAT&q)
	{
		m=Vec4(q.m);
		return (*this);
	}

	inline QUAT QUAT::conjugate()
	{
		return QUAT(m(0),-m(1),-m(2),-m(3));
	}

	inline float QUAT::norm()
	{
		return m.norm();
	}

	inline QUAT QUAT::inverse()
	{
		return conjugate()/(m(0)*m(0)+m(1)*m(1)+m(2)*m(2)+m(3)*m(3));
	}
	inline void QUAT::normalize()
	{
		m.normalize();
	}
	inline QUAT QUAT::normalized()
	{
		return QUAT(m.normalized());
	}

	inline QUAT QUAT::operator/(const float&va)
	{
		return QUAT(m/va);
	}

	bool tri_intersect_test(const Vec3&origin,const Vec3& ray_dir,const Vec3 tri_pt[],float &t,Vec2& coord_para)
	{
		t=0;
		Mat3 vMatrix;
		vMatrix.col(0)=tri_pt[0]-tri_pt[1];
		vMatrix.col(1)=tri_pt[0]-tri_pt[2];
		vMatrix.col(2)=ray_dir;
		Mat3 tmpMat(vMatrix);
		float det_A=vMatrix.determinant();
		if(FCMP(det_A,0))
			return false;
		float inv_det_A=1/det_A;

		float beta,gamma;
		Vec3 solu_vec(tri_pt[0]-origin);
		vMatrix.col(0)=solu_vec;
		beta=vMatrix.determinant()*inv_det_A;
		vMatrix=tmpMat;
		vMatrix.col(1)=solu_vec;
		gamma=vMatrix.determinant()*inv_det_A;
		vMatrix=tmpMat;
		vMatrix.col(2)=solu_vec;
		t=vMatrix.determinant()*inv_det_A;
		coord_para=Vec2(beta,gamma);
		if(beta>=0&&gamma>=0&&beta+gamma<=1.0f)
			return true;
		return false;
	}
}