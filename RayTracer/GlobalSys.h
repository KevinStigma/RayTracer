#ifndef  RAYTRACER_GLOBALSYS_H
#define RAYTRACER_GLOBALSYS_H
#include "zyk/Data_structure.h"

#define MAX_MATERIAL 10

class CGlobalSys
{
public:
	CGlobalSys();
	void init_Light();
	void init_Material();

	float viewport_width,viewport_height,pixel_num;
	zyk::Camera m_cam;
	zyk::Light m_light;
	zyk::Material m_materials[MAX_MATERIAL];
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};
extern CGlobalSys *g_pGlobalSys;
#endif