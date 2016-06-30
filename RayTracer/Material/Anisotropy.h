#ifndef MATERIAL_ANISTROPY_H
#define MATERIAL_ANISTROPY_H
#include "..\ZYK\Data_structure.h"
struct Intersection_Info;
class Anistropy:public zyk::Material
{
public:
	virtual Vec4 render(const std::vector<zyk::Object*>&p_objects, const Intersection_Info& inter_info,
		const Vec3& ray_dir, int depth, float input_rei);
};

#endif
