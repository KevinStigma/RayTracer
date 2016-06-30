#include "Mirror.h"
#include "..\IntersectionInfo.h"
#include "..\InputRay.h"
#include "..\GlobalSys.h"
Vec4 Mirror::render(const std::vector<zyk::Object*>&p_objects, const Intersection_Info& inter_info,
	const Vec3& ray_dir, int depth, float input_rei)
{
	Vec3 reflect_dir = (ray_dir - 2 * ray_dir.dot(inter_info.inter_nor)*inter_info.inter_nor).normalized();
	Vec4 shade_color =g_pGlobalSys->castRayShading_McSampling(p_objects, InputRay(inter_info.inter_pt + 
		reflect_dir*0.01, reflect_dir), depth + 1);
	return shade_color;
}
