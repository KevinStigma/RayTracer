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

	float calCocovariance(const MatrixXf&pMat,const std::vector<float>&pAvg,int x_id,int y_id)
	{
		int num=pMat.rows();
		float s=0;
		for(int i=0;i<num;i++)
			s+=(pMat(i,x_id)-pAvg[x_id])*(pMat(i,y_id)-pAvg[y_id]);
		s/=(num-1);
		return s;
	}

	void PCAfor3D(const float pData[],int dataNum,Vec3& eigenValues,Mat3& eigenVector)
	{
		static const int dimension=3;
		MatrixXf data_mat(dataNum,3);
		for(int i=0;i<dataNum;i++)
		{
			for(int j=0;j<dimension;j++)
				data_mat(i,j)=pData[i*3+j];
		}
		std::vector<float> avg(dimension);
		for(int i=0;i<dimension;i++)
			avg[i]=(data_mat.col(i).sum())/((float)dataNum);
		
		Mat3 covMatrix;
		for(int i=0;i<dimension;i++)
			for(int j=0;j<dimension;j++)
				covMatrix(i,j)=calCocovariance(data_mat,avg,i,j);

		EigenSolver<Mat3>es(covMatrix);
		for(int i=0;i<3;i++)
		{
			eigenValues(i)=es.eigenvalues()[i].real();
			for(int j=0;j<3;j++)
				eigenVector(i,j)=es.eigenvectors().col(j)(i).real();
		}
	}

	//transform the vector dir(in local coordinate) into world coordinate based on 
	//a normal vector
	Vec3 transCoordinate(const Vec3& normal, const Vec3&dir)
	{
		Vec3 Nt, Nb;
		if (std::fabs(normal(0))>std::fabs(normal(1)))
			Nt = Vec3(normal(2), 0, -normal(0)).normalized();
		else
			Nt = Vec3(0, -normal(2), normal(1)).normalized();
		Nb = normal.cross(Nt);
		Mat3 trans_mat;
		trans_mat.col(0) = Nt;
		trans_mat.col(1) = normal;
		trans_mat.col(2) = Nb;
		return trans_mat*dir;
	}
}