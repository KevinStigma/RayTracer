#ifndef MATHEMATICS_H
#define MATHEMATICS_H
#include "Data_structure.h"

namespace zyk
{
	//quaternion implementation
	struct QUAT
	{
		QUAT():m(0,0,0,0){}
		QUAT(float a,float b,float c,float d):m(a,b,c,d){}
		QUAT(const Vec4& me):m(me){}
		QUAT(const QUAT&q):m(q.m){}

		QUAT operator+(const QUAT&q);
		QUAT operator-(const QUAT&q);
		QUAT operator*(const QUAT&q);
		QUAT operator*(const float&val);
		QUAT operator/(const float&va);
		QUAT operator=(const QUAT&q);
		QUAT conjugate();
		float norm();
		QUAT inverse();
		void normalize();
		QUAT normalized();
		Vec4 m;
		static QUAT Identify;
		static Vec3 rotate(float angle,const Vec3& axis,const Vec3&point);
		static Vec4 rotate(float angle,const Vec3& axis,const Vec4&point);
	};

	//intersection test for triangle
	//coor_para means the parameters in the triangle coordinate:
	//p=a+beta*(b-a)+gamma*(c-a),coord_para(0)=beta and coord_para(1)=gamma
	bool tri_intersect_test(const Vec3&origin,const Vec3& ray_dir,const Vec3 tri_pt[],float &t,Vec2& coord_para);
	void PCAfor3D(const float pData[],int dataNum,Vec3& eigenValues,Mat3& eigenVector);
	float calCocovariance(const MatrixXf&pMat,const std::vector<float>&pAvg,int x_id,int y_id);
	Vec3 transCoordinate(const Vec3& normal, const Vec3&dir);
}
#endif