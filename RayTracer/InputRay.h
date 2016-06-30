#ifndef INPUTRAY_H
#define INPUTRAY_H
#include "zyk\Data_structure.h"
struct InputRay
{
	Vec3 origin;
	Vec3 dir;
	InputRay(Vec3 o = Vec3(0, 0, 0), Vec3 d = Vec3(0, 0, 0)) :origin(o), dir(d){}
};
#endif