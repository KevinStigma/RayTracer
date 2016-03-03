#ifndef  RAYTRACER_GLOBALSYS_H
#define RAYTRACER_GLOBALSYS_H
#include "zyk/Data_structure.h"
class CGlobalSys
{
public:
	CGlobalSys();
	float viewport_width,viewport_height,pixel_num;
	zyk::Camera m_cam;
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
extern CGlobalSys *g_pGlobalSys;
#endif