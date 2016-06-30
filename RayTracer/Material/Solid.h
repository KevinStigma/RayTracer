#ifndef MATERIAL_SOLID_H
#define MATERIAL_SOLID_H
#include "..\ZYK\Data_structure.h"
struct Intersection_Info;
namespace zyk
{
	class Object;
};
class Solid :public zyk::Material
{
public:
	Solid(){}
	virtual Vec4 render(const std::vector<zyk::Object*>&p_objects, const Intersection_Info& inter_info,
		const Vec3& ray_dir, int depth, float input_rei);
};

#endif
