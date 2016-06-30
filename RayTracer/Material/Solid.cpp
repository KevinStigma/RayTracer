#include "Solid.h"
#include <stdlib.h>
#include "..\IntersectionInfo.h"
#include "..\InputRay.h"
#include "..\GlobalSys.h"
#include "..\ZYK\mathematics.h"

//generate a random direction in a hemisphere
Vec3 uniformSampleHemisphere(const float &r1, const float &r2)
{
	float sinTheta = sqrtf(1 - r1*r1);
	float phi = 2 * M_PI * r2;
	float x = sinTheta * cosf(phi);
	float z = sinTheta * sinf(phi);
	return Vec3(x, r1, z);
}

Vec4 Solid::render(const std::vector<zyk::Object*>&p_objects, const Intersection_Info& inter_info,
	const Vec3& ray_dir, int depth, float input_rei)
{
	static float inv_rand_max = 1 / (float)RAND_MAX;
	float r1 = rand() *inv_rand_max;
	float r2 = rand()*inv_rand_max;
	
	Vec3 sample_dir = zyk::transCoordinate(inter_info.inter_nor, uniformSampleHemisphere(r1, r2));

	Vec4 c = g_pGlobalSys->castRayShading_McSampling(p_objects,InputRay(inter_info.inter_pt + sample_dir*0.01, sample_dir), 
		depth + 1,input_rei);
	Vec4 shade_color = zyk::dot_multV4(r1*c, rd);
	return shade_color;
}
