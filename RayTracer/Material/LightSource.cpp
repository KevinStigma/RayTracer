#include "LightSource.h"
#include "..\IntersectionInfo.h"
#include "..\InputRay.h"
#include "..\GlobalSys.h"
Vec4 LightSource::render(const std::vector<zyk::Object*>&p_objects, const Intersection_Info& inter_info,
	const Vec3& ray_dir, int depth, float input_rei)
{
	return Vec4(g_pGlobalSys->light_intensity, g_pGlobalSys->light_intensity, g_pGlobalSys->light_intensity, 1.0f);
}