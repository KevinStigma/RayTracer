#ifndef INTERSECTION_INFO_H
#define INTERSECTION_INFO_H
#include "zyk\Data_structure.h"
struct Intersection_Info
{
	Vec3 inter_pt;
	Vec3 inter_nor;
	zyk::Material* material;
	Intersection_Info() :material(NULL){}
};
#endif