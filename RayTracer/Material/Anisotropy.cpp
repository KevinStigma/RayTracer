#include "Anisotropy.h"
#include <stdlib.h>
#include <windows.h>
#include <WinBase.h>
#include "..\IntersectionInfo.h"
#include "..\InputRay.h"
#include "..\GlobalSys.h"
#include "..\ZYK\mathematics.h"

Vec3 computeHalfVec(const zyk::Material* material, float r1, float r2)
{
	float exponent = material->rough_exponent;
	float costheta = pow(r1, 1.0f / (exponent + 1));
	float sintheta = sqrt(max(0.0f, 1.0f - costheta*costheta));
	float phi = r2*2.0f*M_PI;
	return Vec3(sintheta*cosf(phi), costheta, sintheta*sinf(phi));//spherical direction
}


Vec4 Anistropy::render(const std::vector<zyk::Object*>&p_objects, const Intersection_Info& inter_info,
	const Vec3& ray_dir, int depth, float input_rei)
{
	static float inv_rand_max = 1.0f/RAND_MAX;
	float r1 = rand()*inv_rand_max;
	float r2 = rand()*inv_rand_max;

	Vec3 h = zyk::transCoordinate(inter_info.inter_nor, computeHalfVec(this, r1, r2));
	h.normalize();
	Vec3 n = inter_info.inter_nor;
	Vec3 wo = -ray_dir;

	if (h.dot(wo) <0.0f)
		h = -h;
	if (n.dot(h) < 0.0f)
		n = -n;
	if (wo.dot(h) < 0.0f)
		return Vec4(0,0,0,1);

	Vec3  wi = h * (2 * wo.dot(h)) - wo;
	float G = std::min<float>(1.0,
		std::min<float>(2.0*n.dot(h)*n.dot(wo) / wo.dot(h),
		2.0f*n.dot(h)*n.dot(wi) / wo.dot(h)));

	float exponent = rough_exponent;
	float D = (exponent + 2)*pow(h.dot(n), exponent) / (2 * M_PI);
	Vec4 c = g_pGlobalSys->castRayShading_McSampling(p_objects, InputRay(inter_info.inter_pt + wi*0.01, wi), depth + 1);
	Vec4 brdf = rd*(D * G / (4 * n.dot(wo)*n.dot(wi)));
	Vec4 shade_color = zyk::dot_multV4(brdf, c)*max(0, n.dot(wi));
	return shade_color;
}
