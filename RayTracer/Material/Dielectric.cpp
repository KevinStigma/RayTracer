#include "Dielectric.h"
#include "..\IntersectionInfo.h"
#include "..\InputRay.h"
#include "..\GlobalSys.h"

void fresnel(float cos1, float cos2, const float rei[], float &kr)
{
	//in fact, this function should check if there exists total internal reflection,
	//but now that its father function refractRay has this detection, so I don't consider
	//this situation in the function
	float fr1, fr2;
	fr1 = (rei[1] * cos1 - rei[0] * cos2) / (rei[1] * cos1 + rei[0] * cos2);
	fr2 = (rei[0] * cos2 - rei[1] * cos1) / (rei[0] * cos2 + rei[1] * cos1);
	kr = (fr1*fr1 + fr2*fr2)*0.5f;
}

bool refractRay(const Vec3& origin, const Vec3&incident_dir, const Vec3& normal, const float rei[],
	Vec3& refract_dir, float& ref_weight)
{
	float eta = rei[0] / rei[1];
	float c1 = normal.dot(incident_dir), c2 = 1 - eta*eta*(1 - c1*c1);
	if (c2<0)
	{
		ref_weight = 1.0f;
		return false;
	}

	Vec3 v_nor = normal;
	if (c1<0.0f)
		c1 = -c1;
	else
		v_nor *= -1;
	refract_dir = eta*incident_dir + (eta*c1 - sqrt(c2))*v_nor;
	fresnel(c1, fabs(refract_dir.dot(normal)), rei, ref_weight);
	return true;
}

Vec4 Dielectric::render(const std::vector<zyk::Object*>&p_objects, const Intersection_Info& inter_info,
	const Vec3& ray_dir, int depth, float input_rei)
{
	float ioRei[2] = { input_rei, rei };
	if (FCMP(ioRei[0], ioRei[1]) && ioRei[0]>1.0f)// the ray is leaving out the dielectric
		ioRei[1] = 1.0f;

	Vec3 refra_dir;
	Vec4 shade_color;
	float ref_weight, refra_weight;
	bool is_refract = refractRay(inter_info.inter_pt, ray_dir,inter_info.inter_nor, ioRei, refra_dir, ref_weight);
	if (is_refract)
	{
		float out_rei = ioRei[1];
		shade_color = g_pGlobalSys->castRayShading_McSampling(p_objects,
			InputRay(inter_info.inter_pt + refra_dir*0.01, refra_dir),depth + 1, out_rei);
	}
	else
		shade_color = Vec4(0,0,0,1);
	return shade_color;
}
